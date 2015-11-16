/**
Software License Agreement (proprietary)

\file      buffer_writer_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using namespace eip::serialization;

class BufferWriterTest : public :: testing :: Test
{

};

TEST_F(BufferWriterTest, test_write_int)
{
  unsigned char d[8];
  shared_ptr<Writer> writer = make_shared<BufferWriter> (buffer(d));
  EIP_UDINT value = 0x123455AA;
  writer->write(value);
  value = 0xDEADBEEF;
  writer->write(value);

  EXPECT_EQ(8, writer->getByteCount());
  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(0x34, d[2]);
  EXPECT_EQ(0x12, d[3]);
  EXPECT_EQ(0xEF, d[4]);
  EXPECT_EQ(0xBE, d[5]);
  EXPECT_EQ(0xAD, d[6]);
  EXPECT_EQ(0xDE, d[7]);
}

TEST_F(BufferWriterTest, test_write_int_short_buffer)
{
  unsigned char d[7];
  shared_ptr<Writer> writer = make_shared<BufferWriter>(buffer(d));
  EIP_UDINT value = 0x123455AA;
  writer->write(value);
  value = 0xDEADBEEF;
  ASSERT_THROW(writer->write(value), std::length_error);
}

TEST_F(BufferWriterTest, test_write_bytes)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Writer> writer = make_shared<BufferWriter>(buffer(d));
  writer->writeBytes((char*)input, sizeof(input));
  EXPECT_EQ(8, writer->getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(BufferWriterTest, test_write_bytes_short_buffer)
{
  char d[7];
  char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Writer> writer = make_shared<BufferWriter>(buffer(d));
  ASSERT_THROW(writer->writeBytes(input, sizeof(input)), std::length_error);
}

TEST_F(BufferWriterTest, test_write_buffer)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Writer> writer = make_shared<BufferWriter>(buffer(d));
  writer->writeBuffer(buffer(input));
  EXPECT_EQ(8, writer->getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(BufferWriterTest, test_write_buffer_short_buffer)
{
  unsigned char d[7];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Writer> writer = make_shared<BufferWriter>(buffer(d));
  ASSERT_THROW(writer->writeBuffer(buffer(input)), std::length_error);
}
