#include "ReassemblyProcess.h"

ReassemblyProcess::ReassemblyProcess(B headerSize) {
    this->headerSize = headerSize;
}

ReassemblyProcess::~ReassemblyProcess() {
    for(auto it = receivedSdus.begin(); it != receivedSdus.end(); it++) {
        delete *it;
    }
    delete currentPdu;
}

void ReassemblyProcess::addSdu(Packet* sdu) {
    receivedSdus.push_back(sdu);
}

vector<Packet*> ReassemblyProcess::getCompletePdus() {
    vector<Packet*> result;
    if(!currentPdu) {
        currentPdu = new Packet("Reassembled");
    }

    if(receivedSdus.size() == 0) {
        return result;
    }

    for (auto const& sdu : receivedSdus) {
        B offset = B(0);
        MacAddress currentMacAddress = sdu->peekAtFront<RlcHeader>()->getSrc();

        do {
            auto header = sdu->peekAt<RlcHeader>(offset, headerSize);
            B length = header->getLengthField();

            auto data = sdu->peekDataAt(offset + header->getChunkLength(), length);
            currentPdu->insertAtBack(data);

            if(header->getEnd()) {
                result.push_back(currentPdu);
                currentPdu = new Packet("Reassembled");
            }
            offset += (header->getChunkLength() + length);
        } while (offset < sdu->getTotalLength());
    }
    for (auto const& sdu : receivedSdus) {
        delete sdu;
    }
    receivedSdus.clear();
    return result;
}
