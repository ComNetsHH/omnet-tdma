#ifndef __INET_TDMA_I_RLC_H
#define __INET_TDMA_I_RLC_H

#include "inet/common/INETDefs.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/LayeredProtocolBase.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"

using namespace inet;
using namespace std;


/** @brief
 * Interface implementation of the RLC layer. It defines all common function needed to implement a RLC
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */
class IRlc: public LayeredProtocolBase {
protected:
    ~IRlc();

    int upperLayerInGateId = -1;
    int upperLayerOutGateId = -1;
    int lowerLayerInGateId = -1;
    int lowerLayerOutGateId = -1;

    virtual void initialize(int stage) override;
    virtual void sendUp(cMessage *message);
    virtual void sendDown(cMessage *message);

    virtual void handleMessageWhenDown(cMessage *msg) override;
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual bool isInitializeStage(int stage) override { return stage == INITSTAGE_LINK_LAYER; }
    virtual bool isModuleStartStage(int stage) override { return stage == ModuleStartOperation::STAGE_LINK_LAYER; }
    virtual bool isModuleStopStage(int stage) override { return stage == ModuleStopOperation::STAGE_LINK_LAYER; }

    virtual bool isUpperMessage(cMessage *message) override;
    virtual bool isLowerMessage(cMessage *message) override;

    virtual void handleUpperPacket(Packet *packet) override;
    virtual void handleLowerPacket(Packet *packet) override;
    virtual void handleSelfMessage(cMessage *message) override;
};

#endif
