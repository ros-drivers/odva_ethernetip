/**
Software License Agreement (proprietary)

\file      message_router_request_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/message_router_request.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_reader.h"
#include "eip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::MessageRouterRequest;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class MessageRouterRequestTest : public :: testing :: Test
{

};

TEST_F(MessageRouterRequestTest, test_serialize_no_data)
{
  EIP_BYTE d[2];

  MessageRouterRequest data(0xAA);
  EXPECT_EQ(sizeof(d), data.getLength());
  BufferWriter writer(buffer(d));
  data.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // service code
  EXPECT_EQ(0xAA, d[0]);
  // path size is 0
  EXPECT_EQ(   0, d[1]);
}

TEST_F(MessageRouterRequestTest, test_serialize_with_data)
{
  EIP_BYTE d[12];

  EIP_BYTE sample_data[] = {0xEF, 0xBE, 0xAD, 0xDE};
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(sample_data));
  MessageRouterRequest mrr(0xAA);
  mrr.getPath().addLogicalClass(0x73);
  mrr.getPath().addLogicalInstance(1);
  mrr.getPath().addLogicalAttribute(4);
  mrr.setData(sb);
  EXPECT_EQ(sizeof(d), mrr.getLength());
  BufferWriter writer(buffer(d));
  mrr.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // service code
  EXPECT_EQ(0xAA, d[0]);
  // path size is 0
  EXPECT_EQ(   3, d[1]);
  EXPECT_EQ(0x20, d[2]);
  EXPECT_EQ(0x73, d[3]);
  EXPECT_EQ(0x24, d[4]);
  EXPECT_EQ(   1, d[5]);
  EXPECT_EQ(0x30, d[6]);
  EXPECT_EQ(   4, d[7]);
  // sample data
  EXPECT_EQ(0xEF, d[8]);
  EXPECT_EQ(0xBE, d[9]);
  EXPECT_EQ(0xAD, d[10]);
  EXPECT_EQ(0xDE, d[11]);
}
