/**
Software License Agreement (BSD)

\file      rr_data_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "odva_ethernetip/rr_data.h"
#include "odva_ethernetip/serialization/buffer_reader.h"
#include "odva_ethernetip/serialization/buffer_writer.h"

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
