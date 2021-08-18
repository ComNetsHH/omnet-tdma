#import "Rlc.h"
#include "inet/physicallayer/unitdisk/UnitDiskPhyHeader_m.h"
#include "inet/common/ModuleAccess.h"

Define_Module(Rlc);

Rlc::~Rlc() {
}

void Rlc::finish() {
    delete rlcCore;
}

void Rlc::initialize(int stage)
{
    LayeredProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        upperLayerInGateId = findGate("upperLayerIn");
        upperLayerOutGateId = findGate("upperLayerOut");
        lowerLayerInGateId = findGate("lowerLayerIn");
        lowerLayerOutGateId = findGate("lowerLayerOut");

        rlcCore = new RlcCore();
    }
}

void Rlc::sendUp(cMessage *message)
{
    ensureMacAddress();
    if (message->isPacket()) {
        emit(packetSentToUpperSignal, message);
        Packet* fragment = check_and_cast<Packet *>(message);
        fragment->trim();
        rlcCore->addSdu(fragment);

        Packet* tmp = fragment->dup();

        MacAddress srcAddress = fragment->peekAtFront<RlcHeader>()->getSrc();

        vector<Packet*> pdus = rlcCore->getCompletePackets(srcAddress);

        for(int i=0; i< pdus.size(); i++) {
            pdus[i]->copyTags(*tmp);
            send(pdus[i], upperLayerOutGateId);
        }
        delete tmp;
    }
}

void Rlc::ensureMacAddress() {
    if(rlcCore->macAddress == MacAddress::UNSPECIFIED_ADDRESS) {
        auto interfaceEntry = getContainingNicModule(this);
        MacAddress srcAddress = interfaceEntry->getMacAddress();
        rlcCore->macAddress = srcAddress;
    }
}

void Rlc::sendDown(cMessage *message)
{
    ensureMacAddress();

    if (message->isPacket()) {
        emit(packetSentToLowerSignal, message);

       Packet* packet = check_and_cast<Packet *>(message);
       Packet* tmp = packet->dup();
       B size = B(80);

       rlcCore->addPdu(packet);

       while(rlcCore->hasSdu(size)) {
           auto sdu = rlcCore->getSdu(size);
           EV << sdu << endl;
           sdu->copyTags(*tmp);
           send(sdu, lowerLayerOutGateId);
       }
       delete tmp;
    }
}
