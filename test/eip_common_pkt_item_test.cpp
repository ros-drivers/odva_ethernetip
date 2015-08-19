/**
Software License Agreement (proprietary)

\file      eip_common_pkt_item_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_common_pkt_item.h"

using namespace boost::asio;

class EIPCommonPktItemTest : public :: testing :: Test
{

};

TEST_F(EIPCommonPktItemTest, test_default_values)
{
  EIPCommonPktItem item;
  EXPECT_EQ(0, item.getItemType());
  EXPECT_EQ(0, buffer_size(item.getItemData()));
  EXPECT_EQ(NULL, buffer_cast<const void*>(item.getItemData()));
}

TEST_F(EIPCommonPktItemTest, test_simple_value)
{
  char data[] = "abcde";
  EIPCommonPktItem item(0x55AA, buffer(data));
  EXPECT_EQ(0x55AA, item.getItemType());
  EXPECT_EQ(6, buffer_size(item.getItemData()));
  EXPECT_EQ(data, buffer_cast<const void*>(item.getItemData()));
}

TEST_F(EIPCommonPktItemTest, test_serialize_int)
{
  uint64_t data = 0xABCDEF0123456789;
  EIPCommonPktItem item(0xAA55, buffer(&data, sizeof(data)));
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(8, buffer_size(item.getItemData()));
  EXPECT_EQ(&data, buffer_cast<const void*>(item.getItemData()));

  uint8_t dbuf[1024];
  mutable_buffer b1 = buffer(dbuf);
  ASSERT_EQ(12, item.serialize(b1));
  EXPECT_EQ(0x55, dbuf[0]);
  EXPECT_EQ(0xAA, dbuf[1]);
  EXPECT_EQ(0x08, dbuf[2]);
  EXPECT_EQ(0x00, dbuf[3]);
  EXPECT_EQ(0x89, dbuf[4]);
  EXPECT_EQ(0x67, dbuf[5]);
  EXPECT_EQ(0x45, dbuf[6]);
  EXPECT_EQ(0x23, dbuf[7]);
  EXPECT_EQ(0x01, dbuf[8]);
  EXPECT_EQ(0xEF, dbuf[9]);
  EXPECT_EQ(0xCD, dbuf[10]);
  EXPECT_EQ(0xAB, dbuf[11]);
}

TEST_F(EIPCommonPktItemTest, test_serialize_short_buffer)
{
  uint64_t data = 0xABCDEF0123456789;
  EIPCommonPktItem item(0xAA55, buffer(&data, sizeof(data)));
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(8, buffer_size(item.getItemData()));
  EXPECT_EQ(&data, buffer_cast<const void*>(item.getItemData()));

  uint8_t dbuf[8];
  mutable_buffer b1 = buffer(dbuf);
  ASSERT_THROW(item.serialize(b1), std::length_error);
}

TEST_F(EIPCommonPktItemTest, test_deserialize_int)
{
  uint8_t dbuf[] = {0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 
    0xCD, 0xAB};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPktItem item;
  ASSERT_EQ(12, item.deserialize(b1));
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(8, buffer_size(item.getItemData()));
  EXPECT_EQ(0xABCDEF0123456789, *buffer_cast<uint64_t*>(item.getItemData()));
}

TEST_F(EIPCommonPktItemTest, test_deserialize_int_extra_data)
{
  uint8_t dbuf[] = {0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 
    0xCD, 0xAB, 0xFF, 0xFF};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPktItem item;
  ASSERT_EQ(12, item.deserialize(b1));
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(8, buffer_size(item.getItemData()));
  EXPECT_EQ(0xABCDEF0123456789, *buffer_cast<uint64_t*>(item.getItemData()));
}

TEST_F(EIPCommonPktItemTest, test_deserialize_short_throws_exception)
{
  uint8_t dbuf[] = {0x00, 0x00};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPktItem item;
  ASSERT_THROW(item.deserialize(b1), std::length_error);
}

TEST_F(EIPCommonPktItemTest, test_deserialize_short2_throws_exception)
{
  uint8_t dbuf[] = {0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPktItem item;
  ASSERT_THROW(item.deserialize(b1), std::length_error);
}
