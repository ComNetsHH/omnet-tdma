






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

#ifndef __INET_TDMA_RLC_H
#define __INET_TDMA_RLC_H

#include "inet/common/INETDefs.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/LayeredProtocolBase.h"
#include "RlcHeader_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"
#include "RlcCore.h"
#include "IRlc.h"

using namespace inet;
using namespace std;


/** @brief
 * Rlc is the OMNeT++ wrapper implementation of the RLC layer.
 * It borrows its functionality from the RlcCore class
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */
class Rlc : public IRlc, public cListener
{
    protected:
        /** Reference to the RlcCore instance */
        RlcCore* rlcCore = nullptr;

        /** Standard destructor */
        ~Rlc();

        /** Omnet initialization function */
        virtual void initialize(int stage) override;

        /** Omnet cleanup function */
        virtual void finish() override;

        /** Send message up to next layer upstream (IP) */
        virtual void sendUp(cMessage *message) override;

        /** Send message down to next layer downstream (IP) */
        virtual void sendDown(cMessage *message) override;

        /** RLC needs to know the MAC address of the NIC it is in.
         * This function ensures that this value is set and configured in the RlcCore */
        void ensureMacAddress();

    public:
        /** This function returns the current buffer status in bytes
         * \return buffer fill level in bytes
         */
        int getBufferStatus();

        /** Called from the MAC layer to instruct the RLC layer to send down a frame in the required size
         * \param size of the frame in bytes
         */
        void requestFrame(int size);

};


#endif

