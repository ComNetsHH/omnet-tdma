#ifndef __INET_TDMA_RLC_REASSEMBLY_PROCESS_H
#define __INET_TDMA_RLC_REASSEMBLY_PROCESS_H

#ifndef UNIT_TEST
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "RlcHeader_m.h"
#endif

#include <list>

using namespace std;

#ifndef UNIT_TEST
using namespace inet;
#endif

/** @brief
 * When the RLC layer receives fragments it needs to store them and puts them together full IP packet when
 * enough fragments are received. This process is encapsulated in one process per source address since only fragments
 * from the same source can be reassembled.
 *
 *    @author Konrad Fuger, TUHH ComNets
 *    @date August 2020
 */
class ReassemblyProcess {
    protected:
        /** This pointer hold a reference to the current PDU being in reassembly but not complete */
        Packet* currentPdu = nullptr;

        /** List of all SDUs received */
        list<Packet*> receivedSdus;

        /** Size of a RLC header TODO: make configurable */
        B headerSize = B(4);

    public:
        /** Add a sdu to the reassembly process */
        void addSdu(Packet* sdu);

        /** Function to return all completed PDUs */
        vector<Packet*> getCompletePdus();

        /** Standard Constructor */
        ReassemblyProcess(B headerSize);

        /** Standard Destructor */
        ~ReassemblyProcess();
};


#endif
