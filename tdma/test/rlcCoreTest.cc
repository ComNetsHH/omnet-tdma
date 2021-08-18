// include google testing lib
#include <iostream>
#include <gtest/gtest.h>

// include mocked classes
#include "./mock/Packet.h"
#include "./mock/Packet.cc"
#include "./mock/RlcHeader.h"
#include "./mock/RlcHeader.cc"
#include "./mock/Tag.h"
#include "./mock/Tag.cc"
#include "./mock/Chunk.h"
#include "./mock/Chunk.cc"

// include classes to test
#include "../src/rlc/RlcCore.h"
#include "../src/rlc/RlcCore.cc"
#include "../src/rlc/ReassemblyProcess.h"
#include "../src/rlc/ReassemblyProcess.cc"

TEST(RlcSuite, hasSdu_empty)
{
    RlcCore *rlc = new RlcCore();
    EXPECT_EQ(rlc->hasSdu(0), false);
}

TEST(RlcSuite, hasSdu_full)
{
    Packet* pdu = new Packet("");
    RlcCore *rlc = new RlcCore();
    rlc->pduQueue.push_back(pdu);
    EXPECT_EQ(rlc->hasSdu(10), true);
}

TEST(RlcSuite, getSdu_empty)
{
    RlcCore *rlc = new RlcCore();
    EXPECT_EQ(rlc->getSdu(20), nullptr);
}

TEST(RlcSuite, getSdu_full)
{
    Packet* pdu = new Packet("PDU", 1000);
    RlcCore *rlc = new RlcCore();
    rlc->pduQueue.push_back(pdu);
    EXPECT_NE(rlc->getSdu(20), nullptr);
}

TEST(RlcSuite, getSdu_check_size)
{
    Packet* pdu = new Packet("PDU", 64); // 64 bits total size
    RlcCore *rlc = new RlcCore();
    rlc->pduQueue.push_back(pdu);
    EXPECT_EQ(rlc->pduQueue.size(), 1);
    Packet* sdu = rlc->getSdu(30);
    EXPECT_EQ(sdu->getTotalLength(), 30);
    sdu = rlc->getSdu(30);
    EXPECT_EQ(sdu->getTotalLength(), 30);
    sdu = rlc->getSdu(4);
    EXPECT_EQ(sdu->getTotalLength(), 4);
    EXPECT_EQ(rlc->pduQueue.size(), 0);
}
