/**
Software License Agreement (BSD)

\file      identity_item_data_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "ovda_ethernetip/identity_item_data.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

using namespace boost::asio;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class IdentityItemDataTest : public :: testing :: Test
{

};

TEST_F(IdentityItemDataTest, test_deserialize)
{
  EIP_BYTE d[] = {0x55, 0xAA, 0x00, 0x02, 0x12, 0x34, 0xAB, 0xCD, 0xEF, 0x99, 
    0, 0, 0, 0, 0, 0, 0, 0, 0xEE, 0xDD, 0x98, 0x76, 0xA5, 0x5A, 0x6B, 0xC7, 
    0xE7, 0x81, 0x78, 0x56, 0x34, 0x12, 0x06, 'a', 'b', 'c', 'd', 'e', 'f', 0x7E};

  BufferReader reader(buffer(d));
  IdentityItemData id;
  id.deserialize(reader);
  EXPECT_EQ(0xAA55, id.encap_protocol_version);
  EXPECT_EQ(0x0200, id.sockaddr.sin_family);
  EXPECT_EQ(0x3412, id.sockaddr.sin_port);
  EXPECT_EQ(0x99EFCDAB, id.sockaddr.sin_addr.s_addr);
  EXPECT_EQ(0xDDEE, id.vendor_id);
  EXPECT_EQ(0x7698, id.device_type);
  EXPECT_EQ(0x5AA5, id.product_code);
  EXPECT_EQ(0x6B, id.revision[0]);
  EXPECT_EQ(0xC7, id.revision[1]);
  EXPECT_EQ(0x81E7, id.status);
  EXPECT_EQ(0x12345678, id.serial_number);
  EXPECT_STREQ("abcdef", id.product_name.c_str());
  EXPECT_EQ(0x7E, id.state);
  EXPECT_EQ(sizeof(d), id.getLength());
}

TEST_F(IdentityItemDataTest, test_serialize)
{
  IdentityItemData id;
  id.encap_protocol_version = 0xAA55;
  memset(&id.sockaddr, 0, sizeof(sockaddr));
  id.sockaddr.sin_family = 0x0200;
  id.sockaddr.sin_port = 0x3412;
  id.sockaddr.sin_addr.s_addr = 0x99EFCDAB;
  id.vendor_id = 0xDDEE;
  id.device_type = 0x7698;
  id.product_code = 0x5AA5;
  id.revision[0] = 0x6B;
  id.revision[1] = 0xC7;
  id.status = 0x81E7;
  id.serial_number = 0x12345678;
  id.product_name = "abcdef";
  id.state = 0x7E;

  EIP_BYTE d[40];
  BufferWriter writer(buffer(d));
  id.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());
  EXPECT_EQ(d[0], 0x55);
  EXPECT_EQ(d[1], 0xAA);
  EXPECT_EQ(d[2], 0x00);
  EXPECT_EQ(d[3], 0x02);
  EXPECT_EQ(d[4], 0x12);
  EXPECT_EQ(d[5], 0x34);
  EXPECT_EQ(d[6], 0xAB);
  EXPECT_EQ(d[7], 0xCD);
  EXPECT_EQ(d[8], 0xEF);
  EXPECT_EQ(d[9], 0x99);
  EXPECT_EQ(d[10], 0);
  EXPECT_EQ(d[11], 0);
  EXPECT_EQ(d[12], 0);
  EXPECT_EQ(d[13], 0);
  EXPECT_EQ(d[14], 0);
  EXPECT_EQ(d[15], 0);
  EXPECT_EQ(d[16], 0);
  EXPECT_EQ(d[17], 0);
  EXPECT_EQ(d[18], 0xEE);
  EXPECT_EQ(d[19], 0xDD);
  EXPECT_EQ(d[20], 0x98);
  EXPECT_EQ(d[21], 0x76);
  EXPECT_EQ(d[22], 0xA5);
  EXPECT_EQ(d[23], 0x5A);
  EXPECT_EQ(d[24], 0x6B);
  EXPECT_EQ(d[25], 0xC7);
  EXPECT_EQ(d[26], 0xE7);
  EXPECT_EQ(d[27], 0x81);
  EXPECT_EQ(d[28], 0x78);
  EXPECT_EQ(d[29], 0x56);
  EXPECT_EQ(d[30], 0x34);
  EXPECT_EQ(d[31], 0x12);
  EXPECT_EQ(d[32], 0x06);
  EXPECT_EQ(d[33], 'a');
  EXPECT_EQ(d[34], 'b');
  EXPECT_EQ(d[35], 'c');
  EXPECT_EQ(d[36], 'd');
  EXPECT_EQ(d[37], 'e');
  EXPECT_EQ(d[38], 'f');
  EXPECT_EQ(d[39], 0x7E);
}
