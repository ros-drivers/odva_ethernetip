/**
Software License Agreement (proprietary)

\file      rr_data_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "eip/rr_data.h"
#include "eip/serialization/buffer_reader.h"
#include "eip/serialization/buffer_writer.h"

using namespace boost::asio;
using eip::RRData;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

class RRDataTest : public :: testing :: Test
{

};

TEST_F(RRDataTest, test_serialize_no_data)
{
  EIP_BYTE d[16];

  RRData data;
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
  // data length (will be changed later, zero for now)
  EXPECT_EQ(   0, d[14]);
  EXPECT_EQ(   0, d[15]);
}

TEST_F(RRDataTest, test_deserialize_no_data)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    2, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    // data type
    0xB2, 0,
    // data length (currently zero)
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  data.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());
  EXPECT_EQ(0, data.interface_handle);
  EXPECT_EQ(0, data.timeout);
}

TEST_F(RRDataTest, test_deserialize_non_zero_data)
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
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  data.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());
  EXPECT_EQ(0xDEADBEEF, data.interface_handle);
  EXPECT_EQ(0xAA55, data.timeout);
}

TEST_F(RRDataTest, test_deserialize_too_few_items)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    1, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    };

  RRData data;
  BufferReader reader(buffer(d));
  ASSERT_THROW(data.deserialize(reader), std::logic_error);
}
/*
TEST_F(RRDataTest, test_deserialize_too_many_items)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    3, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    // data type
    0xB2, 0,
    // data length (currently zero)
    0, 0,
    // extra item
    0, 0,
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  ASSERT_THROW(data.deserialize(reader), std::logic_error);
}
*/
TEST_F(RRDataTest, test_deserialize_wrong_address_type)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    2, 0, 
    // null address type
    0, 2,
    // null address length
    0, 0, 
    // data type
    0xB2, 0,
    // data length (currently zero)
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  ASSERT_THROW(data.deserialize(reader), std::logic_error);
}

TEST_F(RRDataTest, test_deserialize_contains_address_data)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    2, 0, 
    // null address type
    0, 0,
    // null address length
    2, 0, 
    // example null address data (to make it fail)
    0x55, 0xAA,
    // data type
    0xB2, 0,
    // data length (currently zero)
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  ASSERT_THROW(data.deserialize(reader), std::logic_error);
}

TEST_F(RRDataTest, test_deserialize_wrong_data_type)
{
  EIP_BYTE d[] = {
    // interface handle
    0, 0, 0, 0, 
    // timeout
    0, 0,
    // item count
    2, 0, 
    // null address type
    0, 0,
    // null address length
    0, 0, 
    // data type
    0xB1, 0,
    // data length (currently zero)
    0, 0,
    };

  RRData data;
  BufferReader reader(buffer(d));
  ASSERT_THROW(data.deserialize(reader), std::logic_error);
}
