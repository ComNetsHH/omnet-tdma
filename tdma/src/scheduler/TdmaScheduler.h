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
// author: Konrad Fuger
//

#ifndef __INET_TDMA_SCHEDULER_H
#define __INET_TDMA_SCHEDULER_H

#include "../mac/TdmaMac.h"
#include "inet/common/INETDefs.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/linklayer/acking/AckingMac.h"

using namespace inet;
using namespace std;

/** @brief The TDMAScheduler is a standalone module which handles the
 * assignment of radio resources to individual MAC layer instances.
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */
class TdmaScheduler: public cSimpleModule
{
    protected:
        simsignal_t scheduleSignal;
        simsignal_t utilizationSignal;
        int numNodes = 0;
        int slotIndex = 0;
        map<int, TdmaMac*> clients;
        map<int, int> bufferStatus;

        vector<int> schedule;
        double frameDuration;
        double slotDuration;
        double utilization = 0;
        int frameLength;

        cMessage *schedulingSelfMessage = nullptr;
        cMessage *slotSelfMessage = nullptr;

        void initialize(int stage) override;
        void createSchedule();
        virtual void handleMessage(cMessage *message) override;



  public:
    TdmaScheduler();
    int registerClient(TdmaMac *mac, int bufferStatus);
    void reportBufferStatus(int nodeId, int bufferStatus);
    virtual ~TdmaScheduler();

};


#endif

