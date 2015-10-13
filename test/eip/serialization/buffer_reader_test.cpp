/**
Software License Agreement (proprietary)

\file      buffer_reader_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/serialization/buffer_reader.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using namespace eip::serialization;

class BufferReaderTest : public :: testing :: Test
{

};

TEST_F(BufferReaderTest, test_read_int)
{
  unsigned char d[] = { 0xAA, 0x55, 0x34, 0x12, 0xEF, 0xBE, 0xAD, 0xDE };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(d));
  EIP_UDINT value;
  reader->read(value);
  EXPECT_EQ(0x123455AA, value);
  EXPECT_EQ(4, reader->getByteCount());
  reader->read(value);
  EXPECT_EQ(0xDEADBEEF, value);
  EXPECT_EQ(8, reader->getByteCount());
}

TEST_F(BufferReaderTest, test_read_int_short_buffer)
{
  unsigned char d[] = { 0xAA, 0x55, 0x34, 0x12, 0xEF, 0xBE, 0xAD };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(d));
  EIP_UDINT value;
  reader->read(value);
  EXPECT_EQ(0x123455AA, value);
  ASSERT_THROW(reader->read(value), std::length_error);
}

TEST_F(BufferReaderTest, test_read_bytes)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(input));
  reader->readBytes(d, sizeof(d));
  EXPECT_EQ(8, reader->getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(BufferReaderTest, test_read_bytes_short_buffer)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12 };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(input));
  ASSERT_THROW(reader->readBytes(d, sizeof(d)), std::length_error);
}

TEST_F(BufferReaderTest, test_read_buffer)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(input));
  reader->readBuffer(buffer(d));
  EXPECT_EQ(8, reader->getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(BufferReaderTest, test_read_buffer_short_buffer)
{
  unsigned char d[8];
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12 };
  shared_ptr<Reader> reader = make_shared<BufferReader> (buffer(input));
  ASSERT_THROW(reader->readBuffer(buffer(d)), std::length_error);
}

TEST_F(BufferReaderTest, test_read_bytes_no_copy)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  BufferReader reader (buffer(input));
  unsigned char* d = (unsigned char*)reader.readBytes(sizeof(input));
  EXPECT_EQ(8, reader.getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(BufferReaderTest, test_read_buffer_no_copy)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  BufferReader reader (buffer(input));
  mutable_buffer result = reader.readBuffer(sizeof(input));
  unsigned char* d = buffer_cast<unsigned char*>(result);
  EXPECT_EQ(8, reader.getByteCount());
  EXPECT_EQ(8, buffer_size(result));
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}
