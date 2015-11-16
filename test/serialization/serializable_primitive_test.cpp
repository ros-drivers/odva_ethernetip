/**
Software License Agreement (BSD)

\file      serializable_primitive_test.cpp
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

#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/serializable_primitive.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using namespace boost::iostreams;
using namespace eip::serialization;

class SerializablePrimitiveTest : public :: testing :: Test
{

};

TEST_F(SerializablePrimitiveTest, test_serialize_EIP_UDINT)
{
  shared_ptr<Serializable> sp = make_shared< SerializablePrimitive <EIP_UDINT> > (0xDEADBEEF);

  unsigned char d[4];
  BufferWriter writer(buffer(d));

  sp->serialize(writer);

  EXPECT_EQ(4, writer.getByteCount());
  EXPECT_EQ(0xEF, d[0]);
  EXPECT_EQ(0xBE, d[1]);
  EXPECT_EQ(0xAD, d[2]);
  EXPECT_EQ(0xDE, d[3]);
}

TEST_F(SerializablePrimitiveTest, test_serialize_EIP_UDINT_accessor)
{
  SerializablePrimitive <EIP_UDINT> sp;
  sp.data = 0xDEADBEEF;

  unsigned char d[4];
  BufferWriter writer(buffer(d));

  sp.serialize(writer);

  EXPECT_EQ(4, writer.getByteCount());
  EXPECT_EQ(0xEF, d[0]);
  EXPECT_EQ(0xBE, d[1]);
  EXPECT_EQ(0xAD, d[2]);
  EXPECT_EQ(0xDE, d[3]);
}

TEST_F(SerializablePrimitiveTest, test_serialize_EIP_INT)
{
  shared_ptr<Serializable> sp = make_shared< SerializablePrimitive <EIP_INT> > (-1);

  unsigned char d[2];
  BufferWriter writer(buffer(d));

  sp->serialize(writer);

  EXPECT_EQ(2, writer.getByteCount());
  EXPECT_EQ(0xFF, d[0]);
  EXPECT_EQ(0xFF, d[1]);
}

TEST_F(SerializablePrimitiveTest, test_serialize_EIP_UDINT_short_buffer)
{
  shared_ptr<Serializable> sp = make_shared< SerializablePrimitive <EIP_UDINT> > (0xDEADBEEF);

  unsigned char d[3];
  BufferWriter writer(buffer(d));
  ASSERT_THROW(sp->serialize(writer);, std::length_error);
}

TEST_F(SerializablePrimitiveTest, test_deserialize_EIP_UDINT)
{
  shared_ptr<SerializablePrimitive <EIP_UDINT> > sp = make_shared< SerializablePrimitive <EIP_UDINT> > ();
  shared_ptr<Serializable> s = sp;

  unsigned char d[] = { 0xEF, 0xBE, 0xAD, 0xDE };
  BufferReader reader(buffer(d));

  s->deserialize(reader);

  EXPECT_EQ(4, reader.getByteCount());
  EXPECT_EQ(0xDEADBEEF, sp->data);
}

TEST_F(SerializablePrimitiveTest, test_deserialize_EIP_INT)
{
  shared_ptr<SerializablePrimitive <EIP_INT> > sp = make_shared< SerializablePrimitive <EIP_INT> > ();
  shared_ptr<Serializable> s = sp;

  unsigned char d[] = { 0xFF, 0xFF };
  BufferReader reader(buffer(d));

  s->deserialize(reader);

  EXPECT_EQ(2, reader.getByteCount());
  EXPECT_EQ(-1, sp->data);
}

TEST_F(SerializablePrimitiveTest, test_deserialize_EIP_UDINT_short_buffer)
{
  shared_ptr<SerializablePrimitive <EIP_UDINT> > sp = make_shared< SerializablePrimitive <EIP_UDINT> > ();
  shared_ptr<Serializable> s = sp;

  unsigned char d[] = { 0xEF, 0xBE, 0xAD };
  BufferReader reader(buffer(d));

  ASSERT_THROW(s->deserialize(reader), std::length_error);
}
