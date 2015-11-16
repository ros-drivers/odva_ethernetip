/**
Software License Agreement (BSD)

\file      message_router_response_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/message_router_response.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::MessageRouterResponse;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class MessageRouterResponseTest : public :: testing :: Test
{

};

TEST_F(MessageRouterResponseTest, test_deserialize_no_data)
{
  EIP_BYTE d[] = {0xAA, 0xFF, 0x55, 0};

  MessageRouterResponse data;
  EXPECT_EQ(sizeof(d), data.getLength());
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  // service code
  EXPECT_EQ(0xAA, data.service);
  EXPECT_EQ(0x55, data.general_status);
  EXPECT_FALSE(data.getAdditionalStatus());
  EXPECT_FALSE(data.getResponseData());
}

TEST_F(MessageRouterResponseTest, test_deserialize_additional_status_but_no_data)
{
  EIP_BYTE d[] = {0xAA, 0xFF, 0x55, 2, 0xEF, 0xBE, 0xAD, 0xDE};

  MessageRouterResponse data;
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  // service code
  EXPECT_EQ(0xAA, data.service);
  EXPECT_EQ(0x55, data.general_status);
  ASSERT_TRUE(data.getAdditionalStatus());
  EXPECT_FALSE(data.getResponseData());

  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>
    (data.getAdditionalStatus());
  ASSERT_TRUE(sb);
  EXPECT_EQ(4, sb->getLength());
  EXPECT_EQ(d + 4, buffer_cast<EIP_BYTE*>(sb->getData()));
}

TEST_F(MessageRouterResponseTest, test_deserialize_with_data_but_no_additional)
{
  EIP_BYTE d[] = {0xAA, 0xFF, 0x55, 0, 0xEF, 0xBE, 0xAD, 0xDE};

  MessageRouterResponse data;
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  // service code
  EXPECT_EQ(0xAA, data.service);
  EXPECT_EQ(0x55, data.general_status);
  EXPECT_FALSE(data.getAdditionalStatus());
  ASSERT_TRUE(data.getResponseData());

  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>
    (data.getResponseData());
  ASSERT_TRUE(sb);
  EXPECT_EQ(4, sb->getLength());
  EXPECT_EQ(d + 4, buffer_cast<EIP_BYTE*>(sb->getData()));
}

TEST_F(MessageRouterResponseTest, test_deserialize_with_additional_status_and_data)
{
  EIP_BYTE d[] = {0xAA, 0xFF, 0x55,    2, 0xEF, 0xBE, 0xAD, 0xDE, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  MessageRouterResponse data;
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  // service code
  EXPECT_EQ(0xAA, data.service);
  EXPECT_EQ(0x55, data.general_status);
  EXPECT_TRUE(data.getAdditionalStatus());
  EXPECT_TRUE(data.getResponseData());

  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>
    (data.getAdditionalStatus());
  ASSERT_TRUE(sb);
  EXPECT_EQ(4, sb->getLength());
  EXPECT_EQ(d + 4, buffer_cast<EIP_BYTE*>(sb->getData()));

  sb = boost::dynamic_pointer_cast<SerializableBuffer>(data.getResponseData());
  ASSERT_TRUE(sb);
  EXPECT_EQ(8, sb->getLength());
  EXPECT_EQ(d + 8, buffer_cast<EIP_BYTE*>(sb->getData()));
}
