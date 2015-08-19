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

using namespace boost::asio;

class EIPEncapPktTest : public :: testing :: Test
{

};

TEST_F(EIPEncapPktTest, test_default_values)
{
  EIPEncapPkt pkt;
  EXPECT_EQ(0, pkt.getCommand());
  EXPECT_EQ(0, pkt.getSessionHandle());
  EXPECT_EQ(0, pkt.getStatus());
  EXPECT_EQ(0, pkt.getSenderContext());
  EXPECT_EQ(0, pkt.getOptions());
  EXPECT_EQ(0, pkt.getLength());
}

TEST_F(EIPEncapPktTest, test_constructor)
{
  EIPEncapPkt pkt(0x55AA, 0x87654321);
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0, pkt.getStatus());
  EXPECT_EQ(0, pkt.getSenderContext());
  EXPECT_EQ(0, pkt.getOptions());
  EXPECT_EQ(0, pkt.getLength());
}

TEST_F(EIPEncapPktTest, test_constructor_with_data)
{
  char data[] = "abcdefg";
  EIPEncapPkt pkt(0x55AA, 0x87654321, buffer(data));
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0, pkt.getStatus());
  EXPECT_EQ(0, pkt.getSenderContext());
  EXPECT_EQ(0, pkt.getOptions());
  EXPECT_EQ(8, pkt.getLength());
  EXPECT_EQ(data, buffer_cast<char*>(pkt.getData()));
}

TEST_F(EIPEncapPktTest, test_serialization_simple)
{
  EIPEncapPkt pkt(0x55AA, 0x87654321);
  EIP_USINT d[24];
  ASSERT_EQ(24, pkt.serialize(buffer(d)));
  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(0,   d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x21, d[4]);
  EXPECT_EQ(0x43, d[5]);
  EXPECT_EQ(0x65, d[6]);
  EXPECT_EQ(0x87, d[7]);
  for (size_t i = 8; i < sizeof(d); ++i)
  {
    EXPECT_EQ(0, d[i]);
  }
}

TEST_F(EIPEncapPktTest, test_serialization_simple_short_buffer)
{
  EIPEncapPkt pkt(0x55AA, 0x87654321);
  char d[20];
  ASSERT_THROW(pkt.serialize(buffer(d)), std::length_error);
}

TEST_F(EIPEncapPktTest, test_setters)
{
  EIPEncapPkt pkt;
  pkt.setCommand(0x55AA);
  pkt.setSessionHandle(0x87654321);
  pkt.setStatus(0xFEDCBA98);
  pkt.setSenderContext(0xABCDEF0123456789);
  pkt.setOptions(0xDEADBEEF);

  char data[] = "abcdefg";
  pkt.setData(buffer(data));

  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0xFEDCBA98, pkt.getStatus());
  EXPECT_EQ(0xABCDEF0123456789, pkt.getSenderContext());
  EXPECT_EQ(0xDEADBEEF, pkt.getOptions());
  EXPECT_EQ(8, pkt.getLength());
  EXPECT_EQ(data, buffer_cast<char*>(pkt.getData()));
}

TEST_F(EIPEncapPktTest, test_serialization_complex)
{
  EIPEncapPkt pkt;
  pkt.setCommand(0x55AA);
  pkt.setSessionHandle(0x87654321);
  pkt.setStatus(0xFEDCBA98);
  pkt.setSenderContext(0xABCDEF0123456789);
  pkt.setOptions(0xDEADBEEF);

  char data[] = "abcdefg";
  pkt.setData(buffer(data));

  EIP_USINT d[32];
  ASSERT_EQ(32, pkt.serialize(buffer(d)));

  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(0x08, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x21, d[4]);
  EXPECT_EQ(0x43, d[5]);
  EXPECT_EQ(0x65, d[6]);
  EXPECT_EQ(0x87, d[7]);
  EXPECT_EQ(0x98, d[8]);
  EXPECT_EQ(0xBA, d[9]);
  EXPECT_EQ(0xDC, d[10]);
  EXPECT_EQ(0xFE, d[11]);
  EXPECT_EQ(0x89, d[12]);
  EXPECT_EQ(0x67, d[13]);
  EXPECT_EQ(0x45, d[14]);
  EXPECT_EQ(0x23, d[15]);
  EXPECT_EQ(0x01, d[16]);
  EXPECT_EQ(0xEF, d[17]);
  EXPECT_EQ(0xCD, d[18]);
  EXPECT_EQ(0xAB, d[19]);
  EXPECT_EQ(0xEF, d[20]);
  EXPECT_EQ(0xBE, d[21]);
  EXPECT_EQ(0xAD, d[22]);
  EXPECT_EQ(0xDE, d[23]);
  EXPECT_EQ('a', d[24]);
  EXPECT_EQ('b', d[25]);
  EXPECT_EQ('c', d[26]);
  EXPECT_EQ('d', d[27]);
  EXPECT_EQ('e', d[28]);
  EXPECT_EQ('f', d[29]);
  EXPECT_EQ('g', d[30]);
  EXPECT_EQ(0, d[31]);
}

TEST_F(EIPEncapPktTest, test_serialization_complex_short_buffer)
{
  EIPEncapPkt pkt;
  char data[] = "abcdefg";
  pkt.setData(buffer(data));

  EIP_USINT d[30];
  ASSERT_THROW(pkt.serialize(buffer(d)), std::length_error);
}

TEST_F(EIPEncapPktTest, test_deserialization_simple)
{
  EIPEncapPkt pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0, 0x00, 0x21, 0x43, 0x65, 0x87,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  ASSERT_EQ(24, pkt.deserialize(buffer(d)));
  
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0, pkt.getStatus());
  EXPECT_EQ(0, pkt.getSenderContext());
  EXPECT_EQ(0, pkt.getOptions());
  EXPECT_EQ(0, pkt.getLength());
}

TEST_F(EIPEncapPktTest, test_deserialization_simple_short_buffer)
{
  EIPEncapPkt pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0, 0x00, 0x21, 0x43, 0x65, 0x87,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  ASSERT_THROW(pkt.deserialize(buffer(d)), std::length_error);
}

TEST_F(EIPEncapPktTest, test_deserialization_complex)
{
  EIPEncapPkt pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0x08, 0x00, 0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 
    0xDC, 0xFE, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0xEF, 0xBE, 
    0xAD, 0xDE, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 0, };

  ASSERT_EQ(32, pkt.deserialize(buffer(d)));
  
  EXPECT_EQ(0x55AA, pkt.getCommand());
  EXPECT_EQ(0x87654321, pkt.getSessionHandle());
  EXPECT_EQ(0xFEDCBA98, pkt.getStatus());
  EXPECT_EQ(0xABCDEF0123456789, pkt.getSenderContext());
  EXPECT_EQ(0xDEADBEEF, pkt.getOptions());
  EXPECT_EQ(8, pkt.getLength());
  EXPECT_STREQ("abcdefg", buffer_cast<char*>(pkt.getData()));
}

TEST_F(EIPEncapPktTest, test_deserialization_complex_short_buffer)
{
  EIPEncapPkt pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0x08, 0x00, 0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 
    0xDC, 0xFE, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0xEF, 0xBE, 
    0xAD, 0xDE, 'a', 'b', 'c', 'd', 'e', 'f', 'g', };

  ASSERT_THROW(pkt.deserialize(buffer(d)), std::length_error);
}
