#ifndef __INET_TDMA_RLC_CORE_H
#define __INET_TDMA_RLC_CORE_H

#include <list>
#include <map>
#include "ReassemblyProcess.h"

#ifndef UNIT_TEST
#include "RlcHeader_m.h"
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
using namespace inet;
#endif

using namespace std;


/** @brief RlcCore is a pure C++ implementation of the fragmentation/reassembly logic
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */

class RlcCore {
    protected:
        /** Map to reference reassembly process to MacAdress **/
        map<MacAddress, ReassemblyProcess*> reassemblyProcesses;

        /** Vector to hold all received packets not transmitted yet */
        list<Packet*> pduQueue;

        /** currentPdu in the fragmentation process */
        Packet* currentPdu = nullptr;

        /** number of bits of the current PDU already fragmented */
        b currentPduOffset = b(0);

        /** Header size of the RlcHeader TODO: make configurable */
        B headerSize = B(4);
        uint32_t sequenceNumber = 0;

        /** Helper method to get data and add it to a pdu **/
        void addChunkToPdu(Packet* sdu, b size);


    public:
        /** Standard Constructor */
        RlcCore();

        /** Standard Destructor */
        ~RlcCore();

        /** Adds a received frame to the
         * \param AbstractFrame frame: The frame to be stored
         */
        void addSdu(Packet* sdu);

        /** Adds a packet received from the network layer
         * \param AbstractPacket packet: The packet received
         */
        void addPdu(Packet* pdu);

        /** determines whether there is still a frame to send
         * \return decision whether there is a frame
         */
        bool hasSdu(B size);

        /** returns a frame in the required size
         * \param int size: Size of the required frame.
         */
        Packet* getSdu(b size);

        /** returns all complete packets
         * \return A list of all successfully reassembled packets
         */
        vector<Packet*> getCompletePackets(MacAddress address);

        /** MacAddress of the NIC */
        MacAddress macAddress;
};

#endif
