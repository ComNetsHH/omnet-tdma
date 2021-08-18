#ifndef __MOCK_NET_CHUNK_H
#define __MOCK_NET_CHUNK_H

#include "Definitions.h"

class Chunk {
    private:
        b size = 0;

    public:
        b getTotalLength();
        b getChunkLength();
};

#endif