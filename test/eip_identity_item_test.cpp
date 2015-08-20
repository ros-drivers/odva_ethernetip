/**
Software License Agreement (proprietary)

\file      eip_identity_item_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_identity_item.h"

using namespace boost::asio;

class EIPIdentityItemTest : public :: testing :: Test
{

};

TEST_F(EIPIdentityItemTest, test_default_values)
{
  EIP_BYTE d[] = {0x55, 0xAA, 0x00, 0x02, 0x12, 0x34, 0xAB, 0xCD, 0xEF, 0x99, 
    0, 0, 0, 0, 0, 0, 0, 0, 0xEE, 0xDD, 0x98, 0x76, 0xA5, 0x5A, 0x6B, 0xC7, 
    0xE7, 0x81, 0x78, 0x56, 0x34, 0x12, 0x06, 'a', 'b', 'c', 'd', 'e', 'f', 0x7E};

  EIPIdentityItem id;
  EXPECT_EQ(sizeof(d), id.deserialize(buffer(d)));
  EXPECT_EQ(0xAA55, id.getEncapProtocolVersion());
  EXPECT_EQ(0x0200, id.getSockAddr().sin_family);
  EXPECT_EQ(0x3412, id.getSockAddr().sin_port);
  EXPECT_EQ(0x99EFCDAB, id.getSockAddr().sin_addr.s_addr);
  EXPECT_EQ(0xDDEE, id.getVendorID());
  EXPECT_EQ(0x7698, id.getDeviceType());
  EXPECT_EQ(0x5AA5, id.getProductCode());
  EXPECT_EQ(0x6B, id.getRevision(0));
  EXPECT_EQ(0xC7, id.getRevision(1));
  EXPECT_EQ(0x81E7, id.getStatus());
  EXPECT_EQ(0x12345678, id.getSerialNumber());
  EXPECT_STREQ("abcdef", id.getProductName().c_str());
  EXPECT_EQ(0x7E, id.getState());  
}
