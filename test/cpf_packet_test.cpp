/**
Software License Agreement (proprietary)

\file      cpf_packet_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/cpf_packet.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

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
