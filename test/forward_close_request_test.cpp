/**
Software License Agreement (BSD)

\file      forward_close_request_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/forward_close_request.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

namespace eip {

class ForwardCloseRequestTest : public :: testing :: Test
{

};

TEST_F(ForwardCloseRequestTest, test_serialize)
{
  EIP_BYTE d[20];

  ForwardCloseRequest data;
  data.timeout_tick_size = 6;
  data.timeout_ticks = 40;
  data.connection_sn = 0xAA55;
  data.originator_vendor_id = 0xCBA0;
  data.originator_sn = 0x12345678;
  data.getPath().addLogicalClass(0x04);
  data.getPath().addLogicalInstance(1);
  data.getPath().addLogicalConnectionPoint(0x71);
  data.getPath().addLogicalConnectionPoint(0x66);

  EXPECT_EQ(sizeof(d), data.getLength());
  BufferWriter writer(buffer(d));
  data.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // ticks
  EXPECT_EQ(   6, d[0]);
  EXPECT_EQ(  40, d[1]);
  // connection SN
  EXPECT_EQ(0x55, d[2]);
  EXPECT_EQ(0xAA, d[3]);
  // vendor ID
  EXPECT_EQ(0xA0, d[4]);
  EXPECT_EQ(0xCB, d[5]);
  // originator SN
  EXPECT_EQ(0x78, d[6]);
  EXPECT_EQ(0x56, d[7]);
  EXPECT_EQ(0x34, d[8]);
  EXPECT_EQ(0x12, d[9]);
  // path length
  EXPECT_EQ(   4, d[10]);
  // stupid spot for a reserved byte
  EXPECT_EQ(   0, d[11]);
  // path itself
  EXPECT_EQ(0x20, d[12]);
  EXPECT_EQ(   4, d[13]);
  EXPECT_EQ(0x24, d[14]);
  EXPECT_EQ(   1, d[15]);
  EXPECT_EQ(0x2C, d[16]);
  EXPECT_EQ(0x71, d[17]);
  EXPECT_EQ(0x2C, d[18]);
  EXPECT_EQ(0x66, d[19]);
}

} // namespace eip
