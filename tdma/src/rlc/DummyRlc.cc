#import "DummyRlc.h"

Define_Module(DummyRlc);

DummyRlc::~DummyRlc() {
}

void DummyRlc::initialize(int stage)
{
    LayeredProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        upperLayerInGateId = findGate("upperLayerIn");
        upperLayerOutGateId = findGate("upperLayerOut");
        lowerLayerInGateId = findGate("lowerLayerIn");
        lowerLayerOutGateId = findGate("lowerLayerOut");
    }
}

void DummyRlc::sendUp(cMessage *message)
{
    send(message, upperLayerOutGateId);
}

void DummyRlc::sendDown(cMessage *message)
{
    send(message, lowerLayerOutGateId);
}
