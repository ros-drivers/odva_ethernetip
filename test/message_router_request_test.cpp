/**
Software License Agreement (BSD)

\file      message_router_request_test.cpp
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
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/message_router_request.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

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
