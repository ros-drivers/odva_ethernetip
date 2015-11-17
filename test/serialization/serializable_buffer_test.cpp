/**
Software License Agreement (BSD)

\file      serializable_buffer_test.cpp
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
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include "odva_ethernetip/serialization/buffer_writer.h"
#include "odva_ethernetip/serialization/buffer_reader.h"
#include "odva_ethernetip/serialization/stream_reader.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using namespace boost::iostreams;
using namespace eip::serialization;

class SerializableBufferTest : public :: testing :: Test
{

};

TEST_F(SerializableBufferTest, test_serialize)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Serializable> sb = make_shared<SerializableBuffer> (buffer(input));

  unsigned char d[8];
  BufferWriter writer(buffer(d));

  sb->serialize(writer);

  EXPECT_EQ(8, writer.getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
  EXPECT_EQ(0xFB, d[4]);
  EXPECT_EQ(0xFA, d[5]);
  EXPECT_EQ(0x12, d[6]);
  EXPECT_EQ(0x34, d[7]);
}

TEST_F(SerializableBufferTest, test_write_bytes_short_buffer)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  shared_ptr<Serializable> sb = make_shared<SerializableBuffer> (buffer(input));

  unsigned char d[7];
  BufferWriter writer(buffer(d));

  ASSERT_THROW(sb->serialize(writer);, std::length_error);
}

TEST_F(SerializableBufferTest, test_deserialize)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  BufferReader reader(buffer(input));

  unsigned char d[8];
  shared_ptr<Serializable> sb = make_shared<SerializableBuffer> (buffer(d));

  sb->deserialize(reader);

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

TEST_F(SerializableBufferTest, test_deserialize_fixed_length)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  basic_array_source<char> sr((const char*)input, sizeof(input));
  shared_ptr<std::istream> isp = make_shared< stream< basic_array_source<char> > > (sr);
  StreamReader reader(isp);

  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer> ();
  shared_ptr<Serializable> s = sb;

  s->deserialize(reader, 4);
  EXPECT_EQ(4, reader.getByteCount());

  unsigned char* d = buffer_cast<unsigned char*>(sb->getData());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFE, d[1]);
  EXPECT_EQ(0xFD, d[2]);
  EXPECT_EQ(0xFC, d[3]);
}

TEST_F(SerializableBufferTest, test_deserialize_no_copy)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  BufferReader reader(buffer(input));

  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer> ();
  shared_ptr<Serializable> s = sb;
  s->deserialize(reader, 4);

  EXPECT_EQ(4, reader.getByteCount());
  EXPECT_EQ(4, buffer_size(sb->getData()));
  unsigned char* d = buffer_cast<unsigned char*>(sb->getData());
  EXPECT_EQ(input, d);
}

TEST_F(SerializableBufferTest, test_deserialize_short_buffer)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12 };
  BufferReader reader(buffer(input));

  unsigned char d[8];
  shared_ptr<Serializable> sb = make_shared<SerializableBuffer> (buffer(d));

  ASSERT_THROW(sb->deserialize(reader), std::length_error);
}

TEST_F(SerializableBufferTest, test_deserialize_no_copy_short_buffer)
{
  unsigned char input[] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0x12, 0x34 };
  BufferReader reader(buffer(input));

  shared_ptr<Serializable> sb = make_shared<SerializableBuffer> ();
  ASSERT_THROW(sb->deserialize(reader, 9), std::length_error);
}
