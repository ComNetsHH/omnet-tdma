#include "Packet.h"

Packet::Packet(const char *name) {

}

Packet::Packet(const Packet& other) {
    
}

Packet::Packet(const char *name, b size) {

}

b Packet::getTotalLength() {
    return 0;
}

Ptr<Chunk> Packet::peekDataAt(b start, b end) {
    return chunks.front();
}

void Packet::insertAtFront(Ptr<Chunk> chunk) {
    chunks.push_front(chunk);
}

void Packet::insertAtBack(Ptr<Chunk> chunk) {
    chunks.push_back(chunk);
}

template <typename T>
Ptr<T> Packet::getTag() {
    Ptr<Tag> tag = tags.front();
    return static_pointer_cast<T>(tag);
}

template <typename T>
Ptr<T> Packet::peekAtFront(b length, int flags) {
    Ptr<Chunk> chunk = chunks.front();
    return static_pointer_cast<T>(chunk);
}

template <typename T>
Ptr<T> Packet::peekAt(b start, b size) {
    return this->peekAtFront<T>();
}

