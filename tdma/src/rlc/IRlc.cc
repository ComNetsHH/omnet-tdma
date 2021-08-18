#include "IRlc.h"

void IRlc::initialize(int stage) {

}

void IRlc::sendUp(cMessage *message) {

}
void IRlc::sendDown(cMessage *message) {

}

void IRlc::handleUpperPacket(Packet *packet) {
    sendDown(packet);
}

void IRlc::handleLowerPacket(Packet *packet) {
    sendUp(packet);
}

void IRlc::handleSelfMessage(cMessage *message) {

}

void IRlc::handleMessageWhenDown(cMessage *msg) {

}

void IRlc::handleStartOperation(LifecycleOperation *operation){

}

void IRlc::handleStopOperation(LifecycleOperation *operation) {

}

void IRlc::handleCrashOperation(LifecycleOperation *operation) {

}

bool IRlc::isUpperMessage(cMessage *message)
{
    return message->getArrivalGateId() == upperLayerInGateId;
}

bool IRlc::isLowerMessage(cMessage *message)
{
    return message->getArrivalGateId() == lowerLayerInGateId;
}

IRlc::~IRlc() {

}
