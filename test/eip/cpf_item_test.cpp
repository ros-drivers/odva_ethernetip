/**
Software License Agreement (proprietary)

\file      cpf_item_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/cpf_item.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"

using boost::make_shared;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class CPFItemTest : public :: testing :: Test
{

};

TEST_F(CPFItemTest, test_default_values)
{
  CPFItem item;
  EXPECT_EQ(0, item.getItemType());
  EXPECT_EQ(0, item.getDataLength());
  EXPECT_EQ(4, item.getLength());
  EXPECT_FALSE(item.getData());
}

TEST_F(CPFItemTest, test_simple_value)
{
  char d[] = "abcde";
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(d));
  CPFItem item(0x55AA, sb);
  EXPECT_EQ(0x55AA, item.getItemType());
  EXPECT_EQ(6, item.getDataLength());
  EXPECT_EQ(10, item.getLength());
  EXPECT_EQ(sb, item.getData());
}

TEST_F(CPFItemTest, test_serialize_int)
{
  uint64_t input = 0xABCDEF0123456789;
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(&input, sizeof(input)));
  CPFItem item(0xAA55, sb);
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(12, item.getLength());

  uint8_t d[12];
  BufferWriter writer(buffer(d));
  item.serialize(writer);
  ASSERT_EQ(12, writer.getByteCount());
  EXPECT_EQ(0x55, d[0]);
  EXPECT_EQ(0xAA, d[1]);
  EXPECT_EQ(0x08, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x89, d[4]);
  EXPECT_EQ(0x67, d[5]);
  EXPECT_EQ(0x45, d[6]);
  EXPECT_EQ(0x23, d[7]);
  EXPECT_EQ(0x01, d[8]);
  EXPECT_EQ(0xEF, d[9]);
  EXPECT_EQ(0xCD, d[10]);
  EXPECT_EQ(0xAB, d[11]);
}

TEST_F(CPFItemTest, test_serialize_short_buffer)
{
  uint64_t input = 0xABCDEF0123456789;
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(&input, sizeof(input)));
  CPFItem item(0xAA55, sb);
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(12, item.getLength());

  uint8_t d[11];
  BufferWriter writer(buffer(d));
  ASSERT_THROW(item.serialize(writer), std::length_error);
}

TEST_F(CPFItemTest, test_deserialize_int)
{
  uint8_t input[] = {0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 
    0xCD, 0xAB};
  BufferReader reader(buffer(input));
  CPFItem item;
  item.deserialize(reader);
  ASSERT_EQ(12, reader.getByteCount());
  EXPECT_EQ(0xAA55, item.getItemType());
  EXPECT_EQ(8, item.getDataLength());
  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>(item.getData());
  ASSERT_TRUE(sb);
  EXPECT_EQ(0xABCDEF0123456789, *buffer_cast<uint64_t*>(sb->getData()));
}

TEST_F(CPFItemTest, test_deserialize_short_throws_exception)
{
  uint8_t input[] = {0x00, 0x00};
  BufferReader reader(buffer(input));
  CPFItem item;
  ASSERT_THROW(item.deserialize(reader), std::length_error);
}

TEST_F(CPFItemTest, test_deserialize_short2_throws_exception)
{
  uint8_t input[] = {0x55, 0xAA, 0x08, 0x00, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD};
  BufferReader reader(buffer(input));
  CPFItem item;
  ASSERT_THROW(item.deserialize(reader), std::length_error);
}
