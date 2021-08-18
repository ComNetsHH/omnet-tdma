import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.common.MacAddress;

cplusplus {{
using namespace inet;
}}

// @brief Header for the RLC layer
//
//    @author Konrad Fuger, TUHH ComNets
//    @date August 2020
//
class RlcHeader extends inet::FieldsChunk
{
	uint32_t sequenceNumber;
	bool start;
	bool end;
	inet::B lengthField;
	inet::MacAddress src;
    inet::MacAddress dest;
}

