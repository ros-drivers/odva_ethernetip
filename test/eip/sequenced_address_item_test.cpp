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

#include "eip/sequenced_address_item.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"

using namespace boost::asio;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class SequencedAddressItemTest : public :: testing :: Test
{

};

TEST_F(SequencedAddressItemTest, test_deserialize)
{
  EIP_BYTE d[] = {0x55, 0xAA, 0xEF, 0xBE};

  BufferReader reader(buffer(d));
  SequencedAddressItem item;
  item.deserialize(reader);
  EXPECT_EQ(4, reader.getByteCount());
  EXPECT_EQ(0xAA55, item.connection_id);
  EXPECT_EQ(0xBEEF, item.sequence_num);
  EXPECT_EQ(sizeof(d), item.getLength());
}

TEST_F(SequencedAddressItemTest, test_serialize)
{
  SequencedAddressItem item;
  item.connection_id = 0xBEEF;
  item.sequence_num = 0xABCD;

  EIP_BYTE d[4];
  EXPECT_EQ(sizeof(d), item.getLength());
  BufferWriter writer(buffer(d));
  item.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());
  EXPECT_EQ(d[0], 0xEF);
  EXPECT_EQ(d[1], 0xBE);
  EXPECT_EQ(d[2], 0xCD);
  EXPECT_EQ(d[3], 0xAB);
}
