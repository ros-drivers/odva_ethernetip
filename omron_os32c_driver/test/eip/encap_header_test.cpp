/**
Software License Agreement (proprietary)

\file      encap_header_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>

#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"
#include "eip/encap_header.h"

using namespace eip;
using namespace eip::serialization;

class EncapHeaderTest : public :: testing :: Test
{

};

TEST_F(EncapHeaderTest, test_default_values)
{
  EncapHeader pkt;
  EXPECT_EQ(0, pkt.command);
  EXPECT_EQ(0, pkt.session_handle);
  EXPECT_EQ(0, pkt.status);
  EXPECT_EQ(0, pkt.context[0]);
  EXPECT_EQ(0, pkt.context[1]);
  EXPECT_EQ(0, pkt.options);
  EXPECT_EQ(0, pkt.length);
}

TEST_F(EncapHeaderTest, test_constructor)
{
  EncapHeader pkt(0x55AA, 0x87654321);
  EXPECT_EQ(0x55AA, pkt.command);
  EXPECT_EQ(0x87654321, pkt.session_handle);
  EXPECT_EQ(0, pkt.status);
  EXPECT_EQ(0, pkt.context[0]);
  EXPECT_EQ(0, pkt.context[1]);
  EXPECT_EQ(0, pkt.options);
  EXPECT_EQ(0, pkt.length);
}

TEST_F(EncapHeaderTest, test_serialization_simple)
{
  unsigned char d[24];
  BufferWriter writer(buffer(d));

  EncapHeader pkt(0x55AA, 0x87654321);
  pkt.serialize(writer);
  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(   0, d[2]);
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

TEST_F(EncapHeaderTest, test_serialization_complex)
{
  EncapHeader pkt;
  pkt.command = 0x55AA;
  pkt.session_handle = 0x87654321;
  pkt.status = 0xFEDCBA98;
  pkt.context[0] = 0x23456789;
  pkt.context[1] = 0xABCDEF01;
  pkt.options = 0xDEADBEEF;
  pkt.length = 8;

  EIP_USINT d[24];
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);

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
}

TEST_F(EncapHeaderTest, test_deserialization_simple)
{
  EncapHeader pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0, 0x00, 0x21, 0x43, 0x65, 0x87,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  BufferReader reader(buffer(d));
  pkt.deserialize(reader);

  EXPECT_EQ(0x55AA, pkt.command);
  EXPECT_EQ(0x87654321, pkt.session_handle);
  EXPECT_EQ(0, pkt.status);
  EXPECT_EQ(0, pkt.context[0]);
  EXPECT_EQ(0, pkt.context[1]);
  EXPECT_EQ(0, pkt.options);
  EXPECT_EQ(0, pkt.length);
}

TEST_F(EncapHeaderTest, test_deserialization_complex)
{
  EncapHeader pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0x08, 0x00, 0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 
    0xDC, 0xFE, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0xEF, 0xBE, 
    0xAD, 0xDE, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 0, };

  BufferReader reader(buffer(d));
  pkt.deserialize(reader);

  EXPECT_EQ(0x55AA, pkt.command);
  EXPECT_EQ(0x87654321, pkt.session_handle);
  EXPECT_EQ(0xFEDCBA98, pkt.status);
  EXPECT_EQ(0x23456789, pkt.context[0]);
  EXPECT_EQ(0xABCDEF01, pkt.context[1]);
  EXPECT_EQ(0xDEADBEEF, pkt.options);
  EXPECT_EQ(8, pkt.length);
}
