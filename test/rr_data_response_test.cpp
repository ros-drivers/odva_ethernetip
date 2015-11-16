/**
Software License Agreement (proprietary)

\file      rr_data_response_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/rr_data_response.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::RRDataResponse;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class RRDataResponseTest : public :: testing :: Test
{

};

TEST_F(RRDataResponseTest, test_deserialize_no_data)
{
  EIP_BYTE d[] = {
    // interface handle
    0xEF, 0xBE, 0xAD, 0xDE, 
    // timeout
    0x55, 0xAA,
    // item count
    2, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    // data type
    0xB2, 0,
    // data length (currently zero)
    4, 0,
    // MR response data
    0xAA, 0xFF, 0x55, 0
    };

  RRDataResponse data;
  BufferReader reader(buffer(d));
  data.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());
  EXPECT_EQ(0xDEADBEEF, data.interface_handle);
  EXPECT_EQ(0xAA55, data.timeout);
  EXPECT_EQ(0xAA, data.getServiceCode());
  EXPECT_EQ(0x55, data.getGeneralStatus());
  EXPECT_FALSE(data.getAdditionalStatus());
  EXPECT_FALSE(data.getResponseData());
}

TEST_F(RRDataResponseTest, test_deserialize_with_data)
{
  EIP_BYTE d[] = {
    // interface handle
    0xEF, 0xBE, 0xAD, 0xDE, 
    // timeout
    0x55, 0xAA,
    // item count
    2, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    // data type
    0xB2, 0,
    // data length (currently zero)
    16, 0,
    // MR response data
    0xAA, 0xFF, 0x55,    2, 0xEF, 0xBE, 0xAD, 0xDE, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

  RRDataResponse data;
  BufferReader reader(buffer(d));
  data.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());
  EXPECT_EQ(0xDEADBEEF, data.interface_handle);
  EXPECT_EQ(0xAA55, data.timeout);
  EXPECT_EQ(0xAA, data.getServiceCode());
  EXPECT_EQ(0x55, data.getGeneralStatus());
  EXPECT_TRUE(data.getAdditionalStatus());
  EXPECT_TRUE(data.getResponseData());

  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>
    (data.getAdditionalStatus());
  ASSERT_TRUE(sb);
  EXPECT_EQ(4, sb->getLength());
  EXPECT_EQ(d + 20, buffer_cast<EIP_BYTE*>(sb->getData()));

  sb = boost::dynamic_pointer_cast<SerializableBuffer>(data.getResponseData());
  ASSERT_TRUE(sb);
  EXPECT_EQ(8, sb->getLength());
  EXPECT_EQ(d + 24, buffer_cast<EIP_BYTE*>(sb->getData()));
}
