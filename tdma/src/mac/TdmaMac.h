#ifndef __INET_TDMA_MAC_H
#define __INET_TDMA_MAC_H

#include "inet/common/INETDefs.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/linklayer/acking/AckingMac.h"

using namespace inet;
using namespace std;


class TdmaScheduler;


/** @brief
 * Implementation of the MAC layer
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */
class TdmaMac : public AckingMac
{
    protected:
        /** Reference to the scheduler instance */
        TdmaScheduler* scheduler = nullptr;

        /** Schedule as derived by the scheduler */
        vector<int> schedule;

        /** Id of this mac layer as obtained by the scheduler */
        int nodeId;

        /** Slot duration */
        double slotDuration;

        /** Number of slots per frame */
        int frameLength;

        /** Counter to control retransmissions */
        int currentTransmissionAttemps = 0;

        /** Max number of retransmissions */
        int numRetries;

        /** Self message to recognize timeouts */
        cMessage *transmissionSelfMessage = nullptr;

        /** OMNeT++ initialization function */
        void initialize(int stage) override;

        /** Handle packet from upper layer (RLC) */
        virtual void handleUpperPacket(Packet *packet) override;

        /** Handle message when down */
        virtual void handleMessageWhenDown(cMessage *message) override;

        /** Handle self message */
        virtual void handleSelfMessage(cMessage *message) override;

        /** Callback function for another MAC instance to acknowledge a frame */
        virtual void acked(Packet *frame) override;

        /** Overwritten function to prohibit the radio from causing transmissions */
        void receiveSignal(cComponent *source, simsignal_t signalID, intval_t value, cObject *details) override;

        /** Get a following transmission slot in current frame */
        simtime_t getNextTransmissionSlot();

        /** Get the first slot assigned to me in the next frame */
        simtime_t getFirstSlotInNextFrame();

        /** Check wether there is a grant in the current schedule */
        bool hasGrant();

        /** Check wether there is a grant in the current schedule and wether this one is in the future */
        bool hasFutureGrant();



  public:
    /** Callback function for the scheduler to set a schedule */
    void setSchedule(vector<int> newSchedule);

    /** Standard Constructor */
    TdmaMac();

    /** Standard Destructor */
    virtual ~TdmaMac();

};


#endif

