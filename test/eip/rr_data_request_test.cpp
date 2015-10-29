/**
Software License Agreement (proprietary)

\file      rr_data_request_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/rr_data_request.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_reader.h"
#include "eip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::RRDataRequest;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class RRDataRequestTest : public :: testing :: Test
{

};

TEST_F(RRDataRequestTest, test_serialize_no_data)
{
  EIP_BYTE d[18];

  RRDataRequest data(0xAA);
  EXPECT_EQ(sizeof(d), data.getLength());
  BufferWriter writer(buffer(d));
  data.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // interface handle
  EXPECT_EQ(   0, d[0]);
  EXPECT_EQ(   0, d[1]);
  EXPECT_EQ(   0, d[2]);
  EXPECT_EQ(   0, d[3]);
  // timeout
  EXPECT_EQ(   0, d[4]);
  EXPECT_EQ(   0, d[5]);
  // item count
  EXPECT_EQ(   2, d[6]);
  EXPECT_EQ(   0, d[7]);
  // null address type
  EXPECT_EQ(   0, d[8]);
  EXPECT_EQ(   0, d[9]);
  // null address length
  EXPECT_EQ(   0, d[10]);
  EXPECT_EQ(   0, d[11]);
  // data type
  EXPECT_EQ(0xB2, d[12]);
  EXPECT_EQ(   0, d[13]);
  // data length for empty MR data
  EXPECT_EQ(   2, d[14]);
  EXPECT_EQ(   0, d[15]);
  // service code
  EXPECT_EQ(0xAA, d[16]);
  // path length 0
  EXPECT_EQ(   0, d[17]);
}

TEST_F(RRDataRequestTest, test_serialize_with_data)
{
  EIP_BYTE d[28];

  RRDataRequest data(0xAA);
  EIP_BYTE sample_data[] = {0xEF, 0xBE, 0xAD, 0xDE};
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(sample_data));
  data.getPath().addLogicalClass(0x73);
  data.getPath().addLogicalInstance(1);
  data.getPath().addLogicalAttribute(4);
  data.setData(sb);

  EXPECT_EQ(sizeof(d), data.getLength());
  BufferWriter writer(buffer(d));
  data.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // interface handle
  EXPECT_EQ(   0, d[0]);
  EXPECT_EQ(   0, d[1]);
  EXPECT_EQ(   0, d[2]);
  EXPECT_EQ(   0, d[3]);
  // timeout
  EXPECT_EQ(   0, d[4]);
  EXPECT_EQ(   0, d[5]);
  // item count
  EXPECT_EQ(   2, d[6]);
  EXPECT_EQ(   0, d[7]);
  // null address type
  EXPECT_EQ(   0, d[8]);
  EXPECT_EQ(   0, d[9]);
  // null address length
  EXPECT_EQ(   0, d[10]);
  EXPECT_EQ(   0, d[11]);
  // data type
  EXPECT_EQ(0xB2, d[12]);
  EXPECT_EQ(   0, d[13]);
  // data length for MR data
  EXPECT_EQ(   12, d[14]);
  EXPECT_EQ(   0, d[15]);
  // service code
  EXPECT_EQ(0xAA, d[16]);
  // path
  EXPECT_EQ(   3, d[17]);
  EXPECT_EQ(0x20, d[18]);
  EXPECT_EQ(0x73, d[19]);
  EXPECT_EQ(0x24, d[20]);
  EXPECT_EQ(   1, d[21]);
  EXPECT_EQ(0x30, d[22]);
  EXPECT_EQ(   4, d[23]);
  // sample data
  EXPECT_EQ(0xEF, d[24]);
  EXPECT_EQ(0xBE, d[25]);
  EXPECT_EQ(0xAD, d[26]);
  EXPECT_EQ(0xDE, d[27]);
}
