/**
Software License Agreement (proprietary)

\file      sequenced_address_item_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "ovda_ethernetip/sequenced_address_item.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

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
