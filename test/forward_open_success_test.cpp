/**
Software License Agreement (proprietary)

\file      forward_open_success_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/forward_open_success.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::ForwardOpenSuccess;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class ForwardOpenSuccessTest : public :: testing :: Test
{

};

TEST_F(ForwardOpenSuccessTest, test_deserialize_no_data)
{
  EIP_BYTE d[] = {
    // O->T connection ID
    0xAD, 0xDE, 0xEF, 0xBE,
    // T->O connection ID
    0xEF, 0xCD, 0xAB, 0xA9,
    // connection serial number
    0x55, 0xAA,
    // originator vendor ID
    0xA9, 0xCB,
    // originator serial number
    0x12, 0x34, 0x55, 0xAA,
    // O->T Actual Packet Interval
    0x20, 0xA1, 0x07, 0x00,
    // T->O Actual Packet Interval
    0x40, 0x9C, 0x00, 0x00,
    // reply size
    0,
    // reserved byte
    0xFF,
  };

  ForwardOpenSuccess data;
  EXPECT_EQ(sizeof(d), data.getLength());
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  EXPECT_EQ(0xBEEFDEAD, data.o_to_t_connection_id);
  EXPECT_EQ(0xA9ABCDEF, data.t_to_o_connection_id);
  EXPECT_EQ(0xAA55, data.connection_sn);
  EXPECT_EQ(0xCBA9, data.originator_vendor_id);
  EXPECT_EQ(0xAA553412, data.originator_sn);
  EXPECT_EQ(500000, data.o_to_t_api);
  EXPECT_EQ(40000, data.t_to_o_api);
  EXPECT_FALSE(data.getResponseData());
}

TEST_F(ForwardOpenSuccessTest, test_deserialize_with_data)
{
  EIP_BYTE d[] = {
    // O->T connection ID
    0xAD, 0xDE, 0xEF, 0xBE,
    // T->O connection ID
    0xEF, 0xCD, 0xAB, 0xA9,
    // connection serial number
    0x55, 0xAA,
    // originator vendor ID
    0xA9, 0xCB,
    // originator serial number
    0x12, 0x34, 0x55, 0xAA,
    // O->T Actual Packet Interval
    0x20, 0xA1, 0x07, 0x00,
    // T->O Actual Packet Interval
    0x40, 0x9C, 0x00, 0x00,
    // reply size
    4,
    // reserved byte
    0xFF,
    // additional data
    0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xEF,
  };

  ForwardOpenSuccess data;
  BufferReader reader(buffer(d));
  data.deserialize(reader, sizeof(d));
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  EXPECT_EQ(0xBEEFDEAD, data.o_to_t_connection_id);
  EXPECT_EQ(0xA9ABCDEF, data.t_to_o_connection_id);
  EXPECT_EQ(0xAA55, data.connection_sn);
  EXPECT_EQ(0xCBA9, data.originator_vendor_id);
  EXPECT_EQ(0xAA553412, data.originator_sn);
  EXPECT_EQ(500000, data.o_to_t_api);
  EXPECT_EQ(40000, data.t_to_o_api);

  ASSERT_TRUE(data.getResponseData());
  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>
    (data.getResponseData());
  ASSERT_TRUE(sb);
  EXPECT_EQ(8, sb->getLength());
  EXPECT_EQ(d + 26, buffer_cast<EIP_BYTE*>(sb->getData()));
}
