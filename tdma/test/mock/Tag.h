#ifndef __MOCK_NET_TAG_H
#define __MOCK_NET_TAG_H

#include "Definitions.h"

class Tag {

};


class MacAddressReq: public Tag {
    private:
        MacAddress dest;
    public:
        const MacAddress getDestAddress() const;
};

#endif 

