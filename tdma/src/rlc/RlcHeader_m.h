//
// Generated file, do not edit! Created by nedtool 5.6 from rlc/RlcHeader.msg.
//

#ifndef __RLCHEADER_M_H
#define __RLCHEADER_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



class RlcHeader;
#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk

#include "inet/linklayer/common/MacAddress_m.h" // import inet.linklayer.common.MacAddress

// cplusplus {{
using namespace inet;
// }}

/**
 * Class generated from <tt>rlc/RlcHeader.msg:15</tt> by nedtool.
 * <pre>
 * // \@brief Header for the RLC layer
 * //
 * //    \@author Konrad Fuger, TUHH ComNets
 * //    \@date August 2020
 * //
 * class RlcHeader extends inet::FieldsChunk
 * {
 *     uint32_t sequenceNumber;
 *     bool start;
 *     bool end;
 *     inet::B lengthField;
 *     inet::MacAddress src;
 *     inet::MacAddress dest;
 * }
 * </pre>
 */
class RlcHeader : public ::inet::FieldsChunk
{
  protected:
    uint32_t sequenceNumber = 0;
    bool start = false;
    bool end = false;
    inet::B lengthField = B(-1);
    inet::MacAddress src;
    inet::MacAddress dest;

  private:
    void copy(const RlcHeader& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RlcHeader&);

  public:
    RlcHeader();
    RlcHeader(const RlcHeader& other);
    virtual ~RlcHeader();
    RlcHeader& operator=(const RlcHeader& other);
    virtual RlcHeader *dup() const override {return new RlcHeader(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual uint32_t getSequenceNumber() const;
    virtual void setSequenceNumber(uint32_t sequenceNumber);
    virtual bool getStart() const;
    virtual void setStart(bool start);
    virtual bool getEnd() const;
    virtual void setEnd(bool end);
    virtual inet::B getLengthField() const;
    virtual void setLengthField(inet::B lengthField);
    virtual const inet::MacAddress& getSrc() const;
    virtual inet::MacAddress& getSrcForUpdate() { handleChange();return const_cast<inet::MacAddress&>(const_cast<RlcHeader*>(this)->getSrc());}
    virtual void setSrc(const inet::MacAddress& src);
    virtual const inet::MacAddress& getDest() const;
    virtual inet::MacAddress& getDestForUpdate() { handleChange();return const_cast<inet::MacAddress&>(const_cast<RlcHeader*>(this)->getDest());}
    virtual void setDest(const inet::MacAddress& dest);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RlcHeader& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RlcHeader& obj) {obj.parsimUnpack(b);}

#endif // ifndef __RLCHEADER_M_H

