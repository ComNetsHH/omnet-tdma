#ifndef __MOCK_NET_PACKET_H
#define __MOCK_NET_PACKET_H

#include "Chunk.h"
#include "Tag.h"
#include "Definitions.h"
#include <deque>

using namespace std;

class Packet {
    protected:
        deque<Ptr<Chunk>> chunks;
        vector<Ptr<Tag>> tags;
    public:
        explicit Packet(const char *name = nullptr);
        Packet(const char *name, b size);
        Packet(const Packet& other);
        b getTotalLength();

        Ptr<Chunk> peekDataAt(b start, b end);
        void insertAtFront(Ptr<Chunk> chunk);
        void insertAtBack(Ptr<Chunk> chunk);

        template <typename T>
        Ptr<T> peekAtFront(b length = b(-1), int flags = 0);

        template <typename T>
        Ptr<T> peekAt(b start, b size);

        template <typename T>
        Ptr<T> getTag();

};



#endif
