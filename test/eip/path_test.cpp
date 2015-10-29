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

#include "eip/path.h"
#include "eip/serialization/buffer_writer.h"

using namespace boost::asio;
using eip::Path;
using eip::serialization::BufferWriter;

class PathTest : public :: testing :: Test
{

};

TEST_F(PathTest, test_attribute)
{
  EIP_BYTE d[7];

  Path p;
  p.addLogicalClass(0x73);
  p.addLogicalInstance(1);
  p.addLogicalAttribute(4);
  EXPECT_EQ(sizeof(d), p.getLength());
  BufferWriter writer(buffer(d));
  p.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());
  EXPECT_EQ(   3, d[0]);
  EXPECT_EQ(0x20, d[1]);
  EXPECT_EQ(0x73, d[2]);
  EXPECT_EQ(0x24, d[3]);
  EXPECT_EQ(   1, d[4]);
  EXPECT_EQ(0x30, d[5]);
  EXPECT_EQ(   4, d[6]);
}
