#include "RlcCore.h"

using namespace std;
#ifndef UNIT_TEST
using namespace inet;
#endif

RlcCore::RlcCore() {
}

RlcCore::~RlcCore() {
    if(currentPdu != nullptr) {
        delete currentPdu;
    }

    for( auto it = pduQueue.begin(); it != pduQueue.end(); it++) {
        delete *it;
    }

    for (auto it = reassemblyProcesses.begin(); it != reassemblyProcesses.end(); it++ )
    {
        delete it->second;
    }
}

bool RlcCore::hasSdu(B size) {
    if(currentPdu && currentPdu->getTotalLength() - currentPduOffset >= size) {
        return true;
    } else if(pduQueue.size() >= 1) {
        return true;
    }
    return false;
}

void RlcCore::addPdu(Packet* pdu) {
    pduQueue.push_back(pdu);
}

void RlcCore::addSdu(Packet* sdu) {
    MacAddress srcAddress = sdu->peekAtFront<RlcHeader>()->getSrc();
    auto iterator = reassemblyProcesses.find(srcAddress);

    if(iterator != reassemblyProcesses.end())
    {
        iterator->second->addSdu(sdu);
    } else {
        ReassemblyProcess* process = new ReassemblyProcess(headerSize);
        process->addSdu(sdu);
        reassemblyProcesses.insert(pair<MacAddress, ReassemblyProcess*>(srcAddress, process));
    }
}

vector<Packet*> RlcCore::getCompletePackets(MacAddress address) {

    auto iterator = reassemblyProcesses.find(address);
    return iterator->second->getCompletePdus();
}

void RlcCore::addChunkToPdu(Packet* sdu, b size) {
    if(!hasSdu(size)) {
        return;
    }

    if(currentPdu == nullptr) {
        currentPdu = pduQueue.front();
        currentPduOffset = b(0);
    }


    auto macAddressReq = currentPdu->getTag<MacAddressReq>();

    if(sdu->getTotalLength() > B(0)) {
        auto firstHeader = sdu->peekAtFront<RlcHeader>();
        if(firstHeader->getDest() != macAddressReq->getDestAddress()) {
            return;

        }
    }

    b remainingSduSize = size - sdu->getTotalLength() - headerSize;
    b remainingPduSize = currentPdu->getTotalLength() - currentPduOffset;

    if(remainingSduSize <= B(0)) {
        return;
    }

    // The fragment has enough space to add the entire rest of currentPdu
    if(remainingSduSize >= remainingPduSize) {
        auto header = makeShared<RlcHeader>();
        header->setChunkLength(headerSize);
        header->setSequenceNumber(sequenceNumber++);

        auto data = currentPdu->peekDataAt(currentPduOffset, currentPdu->getTotalLength() - currentPduOffset);

        sdu->insertAtFront(data);
        header->setStart(currentPduOffset == B(0));
        header->setEnd(true);
        header->setLengthField(currentPdu->getTotalLength() - currentPduOffset);
        header->setSrc(macAddress);
        header->setDest(macAddressReq->getDestAddress());
        sdu->insertAtFront(header);

        pduQueue.pop_front();
        delete currentPdu;
        currentPdu = nullptr;


    // The fragment can be filled with the rest of currentPdu
    } else {
        auto header = makeShared<RlcHeader>();
        header->setChunkLength(headerSize);
        header->setSequenceNumber(sequenceNumber++);

        auto data = currentPdu->peekDataAt(currentPduOffset, remainingSduSize);

        sdu->insertAtFront(data);

        header->setStart(currentPduOffset == B(0));
        header->setEnd(false);
        header->setLengthField(remainingSduSize);
        header->setSrc(macAddress);
        header->setDest(macAddressReq->getDestAddress());

        sdu->insertAtFront(header);

        currentPduOffset += remainingSduSize;
    }
}

Packet* RlcCore::getSdu(b size) {
    if(!hasSdu(size)) {
        return nullptr;
    }

    Packet* sdu = new Packet("Fragment");

    while(sdu->getTotalLength() < size) {
        b lengthBefore = sdu->getTotalLength();
        addChunkToPdu(sdu, size);
        b lengthAfter = sdu->getTotalLength();

        if(lengthBefore == lengthAfter) {
            // There is nothing to add, return the unfilled sdu
            break;
        }
    }

    return sdu;
}
