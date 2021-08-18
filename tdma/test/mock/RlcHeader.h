#ifndef __MOCK_NET_RLC_HEADER_H
#define __MOCK_NET_RLC_HEADER_H

#include "Chunk.h"

using namespace std;

class RlcHeader: public Chunk {
private:
    MacAddress src;
    MacAddress dest;
    B length;
public:
    void setSequenceNumber(int number);
    void setChunkLength(b size);
    void setEnd(bool end);
    void setStart(bool start);
    void setSrc(MacAddress address);
    void setDest(MacAddress address);
    const MacAddress getSrc() const;
    const MacAddress getDest() const;
    void setLengthField(int length);
    B getLengthField();
    bool getEnd();
};

#endif
