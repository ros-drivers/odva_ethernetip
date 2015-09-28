/**
Software License Agreement (proprietary)

\file      eip_path_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_path.h"

using namespace boost::asio;

class EIPPathTest : public :: testing :: Test
{

};

TEST_F(EIPPathTest, test_attribute)
{
  EIP_BYTE d[7];

  EIPPath p;
  p.addLogicalClass(0x73);
  p.addLogicalInstance(1);
  p.addLogicalAttribute(4);
  EXPECT_EQ(sizeof(d), p.serialize(buffer(d)));
  EXPECT_EQ(   3, d[0]);
  EXPECT_EQ(0x20, d[1]);
  EXPECT_EQ(0x73, d[2]);
  EXPECT_EQ(0x24, d[3]);
  EXPECT_EQ(   1, d[4]);
  EXPECT_EQ(0x30, d[5]);
  EXPECT_EQ(   4, d[6]);
}
