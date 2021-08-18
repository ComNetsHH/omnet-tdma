//
// Copyright (C) 2013 OpenSim Ltd
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
// author: Zoltan Bojthe
//

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../scheduler/TdmaScheduler.h"
#include "TdmaMac.h"
#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/acking/AckingMac.h"
#include "inet/linklayer/acking/AckingMacHeader_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

using namespace inet;
using namespace inet::physicallayer;

Define_Module(TdmaMac);

TdmaMac::TdmaMac()
{
}

TdmaMac::~TdmaMac()
{
    schedule.clear();
    cancelAndDelete(transmissionSelfMessage);
}

void TdmaMac::initialize(int stage)
{
    MacProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        frameLength = par("frameLength");
        slotDuration = par("slotDuration");
        bitrate = par("bitrate");
        headerLength = par("headerLength");
        promiscuous = par("promiscuous");
        fullDuplex = par("fullDuplex");
        useAck = par("useAck");
        ackTimeout = par("ackTimeout");
        numRetries = par("numRetries");

        cModule *radioModule = gate("lowerLayerOut")->getPathEndGate()->getOwnerModule();
        radioModule->subscribe(IRadio::transmissionStateChangedSignal, this);
        radio = check_and_cast<IRadio *>(radioModule);
        transmissionState = IRadio::TRANSMISSION_STATE_UNDEFINED;

        txQueue = check_and_cast<queueing::IPacketQueue *>(getSubmodule("queue"));

        schedule.resize(frameLength, -1);
        scheduler = getModuleFromPar<TdmaScheduler>(par("scheduler"), this);

        // Obtain nodeId by registering the client
        nodeId = scheduler->registerClient(this, 0);
        transmissionSelfMessage = new cMessage("transmission");

        EV << "slotDuration: " << slotDuration << endl;
        EV << "frameLength: " << frameLength << endl;

       }
       else if (stage == INITSTAGE_LINK_LAYER) {
           radio->setRadioMode(fullDuplex ? IRadio::RADIO_MODE_TRANSCEIVER : IRadio::RADIO_MODE_RECEIVER);
           if (useAck) {
               ackTimeoutMsg = new cMessage("link-break");
           }
       }
}

void TdmaMac::handleSelfMessage(cMessage *message)
{
    if (message == ackTimeoutMsg) {
        EV << "AckingMac: timeout: " << currentTxFrame->getFullName() << endl;
        if(currentTransmissionAttemps +1 > numRetries) {
            // packet lost
            emit(linkBrokenSignal, currentTxFrame);
            PacketDropDetails details;
            details.setReason(OTHER_PACKET_DROP);
            dropCurrentTxFrame(details);
            currentTransmissionAttemps = 0;
            EV << "AckingMac: Lost frame" << endl;
        }else {
            EV << "AckingMac: Retrying..." << endl;
            currentTransmissionAttemps++;
        }
    }
    else if(message == transmissionSelfMessage) {
        if (!txQueue->isEmpty()) {
            if(currentTxFrame == nullptr) {
                popTxQueue();
            }
            startTransmitting();
            if(hasFutureGrant()) {
                simtime_t nextTransmissionSlotTime = getNextTransmissionSlot();
                scheduleAt(nextTransmissionSlotTime, transmissionSelfMessage);
            }
        }
    }
    else {
        MacProtocolBase::handleSelfMessage(message);
    }
}
bool TdmaMac::hasGrant() {
    for( int i = 0; i< schedule.size(); i++) {
        if(schedule[i] == nodeId) {
            return true;
        }
    }
    return false;
}


bool TdmaMac::hasFutureGrant() {
    double currentTime = simTime().dbl();
    int currentGlobalSlotIndex = ceil(currentTime / slotDuration);
    double currentSlotIndex = currentGlobalSlotIndex % frameLength;
    int startIndex = currentSlotIndex +1;

    EV << "CurrentSlotIndex: " << currentSlotIndex << " (" << currentGlobalSlotIndex << ")" << endl;

    for( int i = startIndex; i< schedule.size(); i++) {
        if(schedule[i] == nodeId) {
            EV << "Next grant @ slot " << i << endl;
            return true;
        }
    }
    EV << "No future grant, will wait until next scheduling" << endl;
    return false;
}

simtime_t TdmaMac::getFirstSlotInNextFrame() {
    double currentTime = simTime().dbl();
    int currentGlobalSlotIndex = (int)(currentTime / slotDuration);
    int currentFrameIndex = currentGlobalSlotIndex / frameLength;
    int nextFrameindex = currentFrameIndex +1;

    for(int i = 0; i< schedule.size(); i++) {
        if(schedule[i] == nodeId) {
            return (nextFrameindex * frameLength + i) * slotDuration;
        }
    }

    throw cRuntimeError("TdmaMac thinks we have a grant but can't find it");
    return 0;
}

simtime_t TdmaMac::getNextTransmissionSlot() {
    double currentTime = simTime().dbl();
    int currentGlobalSlotIndex = ceil(currentTime / slotDuration);
    double currentSlotIndex = currentGlobalSlotIndex % frameLength;
    int startIndex = currentSlotIndex +1;

    // Loop through the schedule and see if there is a grant
    for(int i = startIndex; i< schedule.size(); i++) {
        if(schedule[i] == nodeId) {
            return (currentGlobalSlotIndex + i - currentSlotIndex) * slotDuration;
        }
    }

    throw cRuntimeError("TdmaMac thinks we have a next grant but can't find it");
    return 0;
}

void TdmaMac::handleUpperPacket(Packet *packet)
{
    txQueue->pushPacket(packet);
    scheduler->reportBufferStatus(nodeId, txQueue->getNumPackets());
}

void TdmaMac::acked(Packet *frame)
{
    Enter_Method_Silent();
    ASSERT(useAck);

    if (currentTxFrame == nullptr) {
        throw cRuntimeError("Unexpected ACK received");
    }

    EV_DEBUG << "AckingMac::acked(" << frame->getFullName() << ") is accepted\n";
    cancelEvent(ackTimeoutMsg);
    deleteCurrentTxFrame();
    scheduler->reportBufferStatus(nodeId, txQueue->getNumPackets());
    currentTransmissionAttemps = 0;
}

void TdmaMac::setSchedule(vector<int> newSchedule) {
    Enter_Method_Silent();
    schedule = newSchedule;
    if(transmissionSelfMessage->isScheduled()) {
        cancelEvent(transmissionSelfMessage);
    }
    if(hasGrant()) {
        simtime_t nextTransmissionTime = getFirstSlotInNextFrame();
        scheduleAt(nextTransmissionTime, transmissionSelfMessage);
    }
}

void TdmaMac::receiveSignal(cComponent *source, simsignal_t signalID, intval_t value, cObject *details) {
    EV <<  "TdmaMac: Ignoring radio initiated transmission" << endl;
}

void TdmaMac::handleMessageWhenDown(cMessage *message) {
    return;
}

