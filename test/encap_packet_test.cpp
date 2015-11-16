/**
Software License Agreement (proprietary)

\file      encap_packet_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include "ovda_ethernetip/encap_packet.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

using boost::make_shared;
using namespace boost::asio;

using namespace eip;
using namespace eip::serialization;

class EncapPacketTest : public :: testing :: Test
{

};

class SerializableDummy : public Serializable
{
public:
  mutable_buffer buf;

  SerializableDummy(mutable_buffer b) : buf(b) { }

  size_t getLength() const
  {
    return buffer_size(buf);
  }

  Writer& serialize(Writer& writer) const
  {
    // printf("Serializing %lu bytes, %s\n", buffer_size(buf), buffer_cast<char*>(buf));
    writer.writeBuffer(buf);
    return writer;
  }

  Reader& deserialize(Reader& reader, size_t length)
  {
    if (length != buffer_size(buf))
    {
      throw std::length_error("Wrong size given to deserialize");
    }
    reader.readBuffer(buf);
    return reader;
  }

  Reader& deserialize(Reader& reader)
  {
    reader.readBuffer(buf);
    return reader;
  }
};

TEST_F(EncapPacketTest, test_default_values)
{
  EncapPacket pkt;
  EXPECT_EQ(0, pkt.getHeader().command);
  EXPECT_EQ(0, pkt.getHeader().session_handle);
  EXPECT_EQ(0, pkt.getHeader().status);
  EXPECT_EQ(0, pkt.getHeader().context[0]);
  EXPECT_EQ(0, pkt.getHeader().context[1]);
  EXPECT_EQ(0, pkt.getHeader().options);
  EXPECT_EQ(0, pkt.getHeader().length);
  EXPECT_EQ(24, pkt.getLength());
}

TEST_F(EncapPacketTest, test_constructor)
{
  EncapPacket pkt(0x55AA, 0x87654321);
  EXPECT_EQ(0x55AA, pkt.getHeader().command);
  EXPECT_EQ(0x87654321, pkt.getHeader().session_handle);
  EXPECT_EQ(0, pkt.getHeader().status);
  EXPECT_EQ(0, pkt.getHeader().context[0]);
  EXPECT_EQ(0, pkt.getHeader().context[1]);
  EXPECT_EQ(0, pkt.getHeader().options);
  EXPECT_EQ(0, pkt.getHeader().length);
  EXPECT_EQ(24, pkt.getLength());
}

TEST_F(EncapPacketTest, test_constructor_with_data)
{
  char data[] = "abcdefg";
  shared_ptr<SerializableBuffer> payload = make_shared<SerializableBuffer>(buffer(data));
  EncapPacket pkt(0x55AA, 0x87654321, payload);
  EXPECT_EQ(0x55AA, pkt.getHeader().command);
  EXPECT_EQ(0x87654321, pkt.getHeader().session_handle);
  EXPECT_EQ(0, pkt.getHeader().status);
  EXPECT_EQ(0, pkt.getHeader().context[0]);
  EXPECT_EQ(0, pkt.getHeader().context[1]);
  EXPECT_EQ(0, pkt.getHeader().options);
  EXPECT_EQ(8, pkt.getHeader().length);
  EXPECT_EQ(32, pkt.getLength());
  EXPECT_EQ(payload, pkt.getPayload());
}

TEST_F(EncapPacketTest, test_serialization_simple)
{
  EncapPacket pkt(0x55AA, 0x87654321);
  EIP_USINT d[24];
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);
  ASSERT_EQ(24, writer.getByteCount());
  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(0,   d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x21, d[4]);
  EXPECT_EQ(0x43, d[5]);
  EXPECT_EQ(0x65, d[6]);
  EXPECT_EQ(0x87, d[7]);
  for (size_t i = 8; i < sizeof(d); ++i)
  {
    EXPECT_EQ(0, d[i]);
  }
}

TEST_F(EncapPacketTest, test_serialization_simple_short_buffer)
{
  EncapPacket pkt(0x55AA, 0x87654321);
  char d[20];
  BufferWriter writer(buffer(d));
  ASSERT_THROW(pkt.serialize(writer), std::length_error);
}

TEST_F(EncapPacketTest, test_serialization_complex)
{
  EncapPacket pkt;
  pkt.getHeader().command = 0x55AA;
  pkt.getHeader().session_handle = 0x87654321;
  pkt.getHeader().status = 0xFEDCBA98;
  pkt.getHeader().context[0] = 0x23456789;
  pkt.getHeader().context[1] = 0xABCDEF01;
  pkt.getHeader().options = 0xDEADBEEF;

  char data[] = "abcdefg";
  shared_ptr<SerializableBuffer> payload = make_shared<SerializableBuffer>(buffer(data));
  pkt.setPayload(payload);

  EIP_USINT d[32];
  BufferWriter writer(buffer(d));
  pkt.serialize(writer);
  ASSERT_EQ(32, writer.getByteCount());

  EXPECT_EQ(0xAA, d[0]);
  EXPECT_EQ(0x55, d[1]);
  EXPECT_EQ(0x08, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x21, d[4]);
  EXPECT_EQ(0x43, d[5]);
  EXPECT_EQ(0x65, d[6]);
  EXPECT_EQ(0x87, d[7]);
  EXPECT_EQ(0x98, d[8]);
  EXPECT_EQ(0xBA, d[9]);
  EXPECT_EQ(0xDC, d[10]);
  EXPECT_EQ(0xFE, d[11]);
  EXPECT_EQ(0x89, d[12]);
  EXPECT_EQ(0x67, d[13]);
  EXPECT_EQ(0x45, d[14]);
  EXPECT_EQ(0x23, d[15]);
  EXPECT_EQ(0x01, d[16]);
  EXPECT_EQ(0xEF, d[17]);
  EXPECT_EQ(0xCD, d[18]);
  EXPECT_EQ(0xAB, d[19]);
  EXPECT_EQ(0xEF, d[20]);
  EXPECT_EQ(0xBE, d[21]);
  EXPECT_EQ(0xAD, d[22]);
  EXPECT_EQ(0xDE, d[23]);
  EXPECT_EQ('a', d[24]);
  EXPECT_EQ('b', d[25]);
  EXPECT_EQ('c', d[26]);
  EXPECT_EQ('d', d[27]);
  EXPECT_EQ('e', d[28]);
  EXPECT_EQ('f', d[29]);
  EXPECT_EQ('g', d[30]);
  EXPECT_EQ(0, d[31]);
}

TEST_F(EncapPacketTest, test_serialization_complex_short_buffer)
{
  EncapPacket pkt;
  char data[] = "abcdefg";
  shared_ptr<SerializableBuffer> payload = make_shared<SerializableBuffer>(buffer(data));
  pkt.setPayload(payload);

  EIP_USINT d[30];
  BufferWriter writer(buffer(d));
  ASSERT_THROW(pkt.serialize(writer), std::length_error);
}

TEST_F(EncapPacketTest, test_deserialization_simple)
{
  EncapPacket pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0, 0x00, 0x21, 0x43, 0x65, 0x87,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  BufferReader reader(buffer(d));
  pkt.deserialize(reader);
  ASSERT_EQ(24, reader.getByteCount());
  
  EXPECT_EQ(0x55AA, pkt.getHeader().command);
  EXPECT_EQ(0x87654321, pkt.getHeader().session_handle);
  EXPECT_EQ(0, pkt.getHeader().status);
  EXPECT_EQ(0, pkt.getHeader().context[0]);
  EXPECT_EQ(0, pkt.getHeader().context[1]);
  EXPECT_EQ(0, pkt.getHeader().options);
  EXPECT_EQ(0, pkt.getHeader().length);
}

TEST_F(EncapPacketTest, test_deserialization_simple_short_buffer)
{
  EncapPacket pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0, 0x00, 0x21, 0x43, 0x65, 0x87,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  BufferReader reader(buffer(d));
  ASSERT_THROW(pkt.deserialize(reader), std::length_error);
}

TEST_F(EncapPacketTest, test_deserialization_complex)
{
  EncapPacket pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0x08, 0x00, 0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 
    0xDC, 0xFE, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0xEF, 0xBE, 
    0xAD, 0xDE, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 0, };

  BufferReader reader(buffer(d));
  pkt.deserialize(reader);
  ASSERT_EQ(32, reader.getByteCount());
  
  EXPECT_EQ(0x55AA, pkt.getHeader().command);
  EXPECT_EQ(0x87654321, pkt.getHeader().session_handle);
  EXPECT_EQ(0xFEDCBA98, pkt.getHeader().status);
  EXPECT_EQ(0x23456789, pkt.getHeader().context[0]);
  EXPECT_EQ(0xABCDEF01, pkt.getHeader().context[1]);
  EXPECT_EQ(0xDEADBEEF, pkt.getHeader().options);
  EXPECT_EQ(8, pkt.getHeader().length);
  shared_ptr<SerializableBuffer> sb = boost::dynamic_pointer_cast<SerializableBuffer>(pkt.getPayload());
  ASSERT_TRUE(sb);
  EXPECT_STREQ("abcdefg", buffer_cast<char*>(sb->getData()));
}

TEST_F(EncapPacketTest, test_deserialization_complex_short_buffer)
{
  EncapPacket pkt;
  EIP_USINT d[] = {0xAA, 0x55, 0x08, 0x00, 0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 
    0xDC, 0xFE, 0x89, 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0xEF, 0xBE, 
    0xAD, 0xDE, 'a', 'b', 'c', 'd', 'e', 'f', 'g', };

  BufferReader reader(buffer(d));
  ASSERT_THROW(pkt.deserialize(reader), std::length_error);
}

TEST_F(EncapPacketTest, test_get_payload_as_sb)
{
  char data[] = "abcdefg";
  shared_ptr<SerializableBuffer> payload = make_shared<SerializableBuffer>(buffer(data));
  EncapPacket pkt(0x55AA, 0x87654321, payload);

  SerializableBuffer sb;
  pkt.getPayloadAs(sb);
  EXPECT_EQ(data, buffer_cast<char*>(sb.getData()));
}

TEST_F(EncapPacketTest, test_get_payload_as_dummy)
{
  char data[] = "abcdefg";
  shared_ptr<SerializableBuffer> payload = make_shared<SerializableBuffer>(buffer(data));
  EncapPacket pkt(0x55AA, 0x87654321, payload);

  char result[8];
  SerializableDummy sd(buffer(result));
  pkt.getPayloadAs(sd);
  EXPECT_STREQ("abcdefg", buffer_cast<char*>(sd.buf));
}

TEST_F(EncapPacketTest, test_get_payload_from_dummy)
{
  char data[] = "abcdefg";
  shared_ptr<SerializableDummy> payload = make_shared<SerializableDummy>(buffer(data));
  EncapPacket pkt(0x55AA, 0x87654321, payload);

  char result[8];
  SerializableDummy sd(buffer(result));
  pkt.getPayloadAs(sd);
  EXPECT_STREQ("abcdefg", buffer_cast<char*>(sd.buf));
}

TEST_F(EncapPacketTest, test_get_payload_from_dummy_to_sb_throws_logic_error)
{
  char data[] = "abcdefg";
  shared_ptr<SerializableDummy> payload = make_shared<SerializableDummy>(buffer(data));
  EncapPacket pkt(0x55AA, 0x87654321, payload);

  SerializableBuffer sb;
  ASSERT_THROW(pkt.getPayloadAs(sb), std::logic_error);
}
