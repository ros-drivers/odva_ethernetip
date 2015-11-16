/**
Software License Agreement (BSD)

\file      serializable_primitive_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
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
