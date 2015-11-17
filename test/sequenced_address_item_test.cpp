/**
Software License Agreement (BSD)

\file      sequenced_address_item_test.cpp
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

#include "odva_ethernetip/sequenced_address_item.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"
#include "odva_ethernetip/serialization/buffer_writer.h"
#include "odva_ethernetip/serialization/buffer_reader.h"

using namespace boost::asio;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class SequencedAddressItemTest : public :: testing :: Test
{

};

TEST_F(SequencedAddressItemTest, test_deserialize)
{
  EIP_BYTE d[] = {0x55, 0xAA, 0xEF, 0xBE, 0xEF, 0xBE, 0xAD, 0xDE};

  BufferReader reader(buffer(d));
  SequencedAddressItem item;
  item.deserialize(reader);
  EXPECT_EQ(8, reader.getByteCount());
  EXPECT_EQ(0xBEEFAA55, item.connection_id);
  EXPECT_EQ(0xDEADBEEF, item.sequence_num);
  EXPECT_EQ(sizeof(d), item.getLength());
}

TEST_F(SequencedAddressItemTest, test_serialize)
{
  SequencedAddressItem item;
  item.connection_id = 0xDEADBEEF;
  item.sequence_num = 0xABCD1234;

  EIP_BYTE d[8];
  EXPECT_EQ(sizeof(d), item.getLength());
  BufferWriter writer(buffer(d));
  item.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());
  EXPECT_EQ(d[0], 0xEF);
  EXPECT_EQ(d[1], 0xBE);
  EXPECT_EQ(d[2], 0xAD);
  EXPECT_EQ(d[3], 0xDE);
  EXPECT_EQ(d[4], 0x34);
  EXPECT_EQ(d[5], 0x12);
  EXPECT_EQ(d[6], 0xCD);
  EXPECT_EQ(d[7], 0xAB);
}
