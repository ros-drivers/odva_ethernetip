/**
Software License Agreement (proprietary)

\file      eip_cpf_packet_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include "os32c/eip_cpf_packet.h"
#include "os32c/eip_cpf_null_item.h"
#include "os32c/eip_cpf_item_primitive.h"

using boost::shared_ptr;
using boost::make_shared;

class EIPCPFPacketTest : public :: testing :: Test
{

};

TEST_F(EIPCPFPacketTest, test_default_values)
{
  EIPCPFPacket pkt;
  EXPECT_EQ(0, pkt.getItemCount());
  EIP_BYTE d[2] = {0xFF, 0xFF};
  boost::iostreams::basic_array_sink<char> sr((char*)d, sizeof(d));  
  boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > os(sr);
  os << pkt;
  EXPECT_EQ(0x00, d[0]);
  EXPECT_EQ(0x00, d[1]);
}

TEST_F(EIPCPFPacketTest, test_serialize_one_item)
{
  shared_ptr<EIPCPFItem> item = make_shared< EIPCPFItemPrimitive<EIP_UDINT> > 
    (0x55AA, 0xDEADBEEF);

  EIPCPFPacket pkt;
  pkt.getItems().push_back(item);

  EIP_BYTE d[10];
  boost::iostreams::basic_array_sink<char> sr((char*)d, sizeof(d));  
  boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > os(sr);
  os << pkt;

  ASSERT_EQ(1, pkt.getItemCount());

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

TEST_F(EIPCPFPacketTest, test_serialize_two_items)
{
  shared_ptr<EIPCPFItem> item1 = make_shared< EIPCPFNullItem > ();
  shared_ptr<EIPCPFItem> item2 = make_shared< EIPCPFItemPrimitive<EIP_UDINT> > 
    (0x55AA, 0xDEADBEEF);

  EIPCPFPacket pkt;
  pkt.getItems().push_back(item1);
  pkt.getItems().push_back(item2);

  EIP_BYTE d[14];
  boost::iostreams::basic_array_sink<char> sr((char*)d, sizeof(d));  
  boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > os(sr);
  os << pkt;

  ASSERT_EQ(2, pkt.getItemCount());

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
