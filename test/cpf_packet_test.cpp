/**
Software License Agreement (BSD)

\file      cpf_packet_test.cpp
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

#include "odva_ethernetip/cpf_packet.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"
#include "odva_ethernetip/serialization/buffer_writer.h"
#include "odva_ethernetip/serialization/buffer_reader.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class CPFPacketTest : public :: testing :: Test
{

};

TEST_F(CPFPacketTest, test_default_values)
{
  CPFPacket pkt;
  EXPECT_EQ(0, pkt.getItemCount());
  EIP_BYTE d[2] = {0xFF, 0xFF};
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);
  EXPECT_EQ(2, writer.getByteCount());
  EXPECT_EQ(0x00, d[0]);
  EXPECT_EQ(0x00, d[1]);
}

TEST_F(CPFPacketTest, test_serialize_one_item)
{
  EIP_UDINT data = 0xDEADBEEF;
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer((char*)&data, sizeof(data)));
  CPFItem item = CPFItem(0x55AA, sb);

  CPFPacket pkt;
  pkt.getItems().push_back(item);
  ASSERT_EQ(1, pkt.getItemCount());

  EIP_BYTE d[10];
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);
  EXPECT_EQ(10, writer.getByteCount());

  // 1 item
  EXPECT_EQ(0x01, d[0]);
  EXPECT_EQ(0x00, d[1]);

  // item with type 0x55AA and length 4
  EXPECT_EQ(0xAA, d[2]);
  EXPECT_EQ(0x55, d[3]);
  EXPECT_EQ(0x04, d[4]);
  EXPECT_EQ(0x00, d[5]);
  EXPECT_EQ(0xEF, d[6]);
  EXPECT_EQ(0xBE, d[7]);
  EXPECT_EQ(0xAD, d[8]);
  EXPECT_EQ(0xDE, d[9]);
}

TEST_F(CPFPacketTest, test_serialize_two_items)
{
  CPFItem item1 = CPFItem();

  EIP_UDINT data = 0xDEADBEEF;
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer((char*)&data, sizeof(data)));
  CPFItem item2 = CPFItem(0x55AA, sb);

  CPFPacket pkt;
  pkt.getItems().push_back(item1);
  pkt.getItems().push_back(item2);
  ASSERT_EQ(2, pkt.getItemCount());

  EIP_BYTE d[14];
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);
  EXPECT_EQ(14, writer.getByteCount());

  // 2 items
  EXPECT_EQ(0x02, d[0]);
  EXPECT_EQ(0x00, d[1]);

  // null item
  EXPECT_EQ(0x00, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x00, d[4]);
  EXPECT_EQ(0x00, d[5]);

  // item with type 0x55AA and length 4
  EXPECT_EQ(0xAA, d[6]);
  EXPECT_EQ(0x55, d[7]);
  EXPECT_EQ(0x04, d[8]);
  EXPECT_EQ(0x00, d[9]);
  EXPECT_EQ(0xEF, d[10]);
  EXPECT_EQ(0xBE, d[11]);
  EXPECT_EQ(0xAD, d[12]);
  EXPECT_EQ(0xDE, d[13]);
}
