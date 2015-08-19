/**
Software License Agreement (proprietary)

\file      eip_common_pkt_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_common_pkt.h"

using namespace boost::asio;

class EIPCommonPktTest : public :: testing :: Test
{

};

TEST_F(EIPCommonPktTest, test_default_values)
{
  EIPCommonPkt pkt;
  EXPECT_EQ(0, pkt.getItemCount());
}

TEST_F(EIPCommonPktTest, test_serialize)
{
  uint64_t data1 = 0xABCDEF0123456789;
  EIPCommonPktItem item1(0xAA55, buffer(&data1, sizeof(data1)));
  char data2[] = "abcde";
  EIPCommonPktItem item2(0x55AA, buffer(data2));

  EIPCommonPkt pkt;
  pkt.getItems().push_back(item1);
  pkt.getItems().push_back(item2);

  ASSERT_EQ(2, pkt.getItemCount());

  uint8_t dbuf[1024];
  mutable_buffer b1 = buffer(dbuf);
  ASSERT_EQ(24, pkt.serialize(b1));

  // 2 items
  EXPECT_EQ(0x02, dbuf[0]);
  EXPECT_EQ(0x00, dbuf[1]);

  // first item with type 0xAA55 and length 8
  EXPECT_EQ(0x55, dbuf[2]);
  EXPECT_EQ(0xAA, dbuf[3]);
  EXPECT_EQ(0x08, dbuf[4]);
  EXPECT_EQ(0x00, dbuf[5]);
  EXPECT_EQ(0x89, dbuf[6]);
  EXPECT_EQ(0x67, dbuf[7]);
  EXPECT_EQ(0x45, dbuf[8]);
  EXPECT_EQ(0x23, dbuf[9]);
  EXPECT_EQ(0x01, dbuf[10]);
  EXPECT_EQ(0xEF, dbuf[11]);
  EXPECT_EQ(0xCD, dbuf[12]);
  EXPECT_EQ(0xAB, dbuf[13]);

  // second item with type 0x55AA and length 6
  EXPECT_EQ(0xAA, dbuf[14]);
  EXPECT_EQ(0x55, dbuf[15]);
  EXPECT_EQ(0x06, dbuf[16]);
  EXPECT_EQ(0x00, dbuf[17]);
  EXPECT_EQ('a',  dbuf[18]);
  EXPECT_EQ('b',  dbuf[19]);
  EXPECT_EQ('c',  dbuf[20]);
  EXPECT_EQ('d',  dbuf[21]);
  EXPECT_EQ('e',  dbuf[22]);
  EXPECT_EQ(0,    dbuf[23]);
}

TEST_F(EIPCommonPktTest, test_serialize_short_buffer)
{
  uint64_t data1 = 0xABCDEF0123456789;
  EIPCommonPktItem item1(0xAA55, buffer(&data1, sizeof(data1)));
  char data2[] = "abcde";
  EIPCommonPktItem item2(0x55AA, buffer(data2));

  EIPCommonPkt pkt;
  pkt.getItems().push_back(item1);
  pkt.getItems().push_back(item2);

  ASSERT_EQ(2, pkt.getItemCount());

  uint8_t dbuf[14];
  mutable_buffer b1 = buffer(dbuf);
  ASSERT_THROW(pkt.serialize(b1), std::length_error);
}

TEST_F(EIPCommonPktTest, test_serialize_very_short_buffer)
{
  EIPCommonPkt pkt;
  uint8_t dbuf[1];
  mutable_buffer b1 = buffer(dbuf);
  ASSERT_THROW(pkt.serialize(b1), std::length_error);
}

TEST_F(EIPCommonPktTest, test_deserialize)
{
  uint8_t dbuf[] = {0x02, 0x00, 0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 
    0x01, 0xEF, 0xCD, 0xAB, 0xAA, 0x55, 0x06, 0x00, 'a', 'b', 'c', 'd', 'e', 0};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPkt pkt;
  ASSERT_EQ(24, pkt.deserialize(b1));
  ASSERT_EQ(2, pkt.getItemCount());

  ASSERT_EQ(0xAA55, pkt.getItems().at(0).getItemType());
  EXPECT_EQ(8, buffer_size(pkt.getItems().at(0).getItemData()));
  EXPECT_EQ(0xABCDEF0123456789, *buffer_cast<uint64_t*>(pkt.getItems().at(0).getItemData()));

  ASSERT_EQ(0x55AA, pkt.getItems().at(1).getItemType());
  EXPECT_EQ(6, buffer_size(pkt.getItems().at(1).getItemData()));
  EXPECT_STREQ("abcde", buffer_cast<char*>(pkt.getItems().at(1).getItemData()));
}

TEST_F(EIPCommonPktTest, test_deserialize_extra_data)
{
  uint8_t dbuf[] = {0x02, 0x00, 0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 
    0x01, 0xEF, 0xCD, 0xAB, 0xAA, 0x55, 0x06, 0x00, 'a', 'b', 'c', 'd', 'e', 0, 
    0xFF, 0xFF, 0xFF, 0xFF};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPkt pkt;
  ASSERT_EQ(24, pkt.deserialize(b1));
  ASSERT_EQ(2, pkt.getItemCount());

  ASSERT_EQ(0xAA55, pkt.getItems().at(0).getItemType());
  EXPECT_EQ(8, buffer_size(pkt.getItems().at(0).getItemData()));
  EXPECT_EQ(0xABCDEF0123456789, *buffer_cast<uint64_t*>(pkt.getItems().at(0).getItemData()));

  ASSERT_EQ(0x55AA, pkt.getItems().at(1).getItemType());
  EXPECT_EQ(6, buffer_size(pkt.getItems().at(1).getItemData()));
  EXPECT_STREQ("abcde", buffer_cast<char*>(pkt.getItems().at(1).getItemData()));
}

TEST_F(EIPCommonPktTest, test_deserialize_empty_buffer)
{
  mutable_buffer b1 = buffer((void*)NULL, 0);
  EIPCommonPkt pkt;
  ASSERT_THROW(pkt.deserialize(b1), std::length_error);
}

TEST_F(EIPCommonPktTest, test_deserialize_short_header)
{
  uint8_t dbuf[] = {0x02};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPkt pkt;
  ASSERT_THROW(pkt.deserialize(b1), std::length_error);
}

TEST_F(EIPCommonPktTest, test_deserialize_short_item_header)
{
  uint8_t dbuf[] = {0x02, 0x00, 0x55, 0xAA, };
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPkt pkt;
  ASSERT_THROW(pkt.deserialize(b1), std::length_error);
}

TEST_F(EIPCommonPktTest, test_deserialize_short_item_data)
{
  uint8_t dbuf[] = {0x02, 0x00, 0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 
    0x01, 0xEF, 0xCD, 0xAB, 0xAA, 0x55, 0x06, 0x00, 'a', 'b'};
  mutable_buffer b1 = buffer(dbuf);
  EIPCommonPkt pkt;
  ASSERT_THROW(pkt.deserialize(b1), std::length_error);
}
