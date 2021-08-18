#include "RlcHeader.h"

void RlcHeader::setSequenceNumber(int number) {

}

void RlcHeader::setChunkLength(b size) {

}

void RlcHeader::setEnd(bool end) {

}

void RlcHeader::setStart(bool start) {

}

void RlcHeader::setLengthField(int length) {

}

void  RlcHeader::setDest(MacAddress address) {

}

void  RlcHeader::setSrc(MacAddress address) {
    
}

const MacAddress RlcHeader::getSrc() const {
    return src;
}

const MacAddress RlcHeader::getDest() const {
    return dest;
}

B RlcHeader::getLengthField() {
    return length;
}

bool RlcHeader::getEnd() {
    return false;
}
