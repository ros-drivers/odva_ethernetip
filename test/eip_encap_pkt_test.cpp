/**
Software License Agreement (proprietary)

\file      eip_encap_pkt_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_encap_pkt.h"

class EIPEncapPktTest : public :: testing :: Test
{

};

TEST_F(EIPEncapPktTest, test_default_values)
{
  EIPEncapPkt pkt;
  boost::asio::const_buffer b = pkt.serialize();
  EXPECT_EQ(24, boost::asio::buffer_size(b));
  const EIP_USINT* p = boost::asio::buffer_cast<const EIP_USINT*>(b);
  for (size_t i = 0; i < boost::asio::buffer_size(b); ++i)
  {
    EXPECT_EQ(0, p[i]);
  }
}

TEST_F(EIPEncapPktTest, test_constructor)
{
  EIPEncapPkt pkt(0x55AA, 0x87654321);
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  boost::asio::const_buffer b = pkt.serialize();
  EXPECT_EQ(24, boost::asio::buffer_size(b));
  const EIP_USINT* p = boost::asio::buffer_cast<const EIP_USINT*>(b);
  EXPECT_EQ(0xAA, p[0]);
  EXPECT_EQ(0x55, p[1]);
  EXPECT_EQ(0,   p[2]);
  EXPECT_EQ(0x00, p[3]);
  EXPECT_EQ(0x21, p[4]);
  EXPECT_EQ(0x43, p[5]);
  EXPECT_EQ(0x65, p[6]);
  EXPECT_EQ(0x87, p[7]);
  for (size_t i = 8; i < boost::asio::buffer_size(b); ++i)
  {
    EXPECT_EQ(0, p[i]);
  }
}

TEST_F(EIPEncapPktTest, test_setters)
{
  EIPEncapPkt pkt;
  pkt.setCommand(0x55AA);
  pkt.setSessionHandle(0x87654321);
  pkt.setStatus(0xFEDCBA98);
  pkt.setOptions(0xDEADBEEF);
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0xFEDCBA98, pkt.getStatus());
  EXPECT_EQ(0xDEADBEEF, pkt.getOptions());
  boost::asio::const_buffer b = pkt.serialize();
  EXPECT_EQ(24, boost::asio::buffer_size(b));
  const EIP_USINT* p = boost::asio::buffer_cast<const EIP_USINT*>(b);
  EXPECT_EQ(0xAA, p[0]);
  EXPECT_EQ(0x55, p[1]);
  EXPECT_EQ(0,   p[2]);
  EXPECT_EQ(0x00, p[3]);
  EXPECT_EQ(0x21, p[4]);
  EXPECT_EQ(0x43, p[5]);
  EXPECT_EQ(0x65, p[6]);
  EXPECT_EQ(0x87, p[7]);
  EXPECT_EQ(0x98, p[8]);
  EXPECT_EQ(0xBA, p[9]);
  EXPECT_EQ(0xDC, p[10]);
  EXPECT_EQ(0xFE, p[11]);
  EXPECT_EQ(0x00, p[12]);
  EXPECT_EQ(0x00, p[13]);
  EXPECT_EQ(0x00, p[14]);
  EXPECT_EQ(0x00, p[15]);
  EXPECT_EQ(0x00, p[16]);
  EXPECT_EQ(0x00, p[17]);
  EXPECT_EQ(0x00, p[18]);
  EXPECT_EQ(0x00, p[19]);
  EXPECT_EQ(0xEF, p[20]);
  EXPECT_EQ(0xBE, p[21]);
  EXPECT_EQ(0xAD, p[22]);
  EXPECT_EQ(0xDE, p[23]);
}


// Run all the tests that were declared with TEST_F()
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
