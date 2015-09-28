/**
Software License Agreement (proprietary)

\file      eip_rr_data_cmd_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/eip_rr_data_cmd.h"

using namespace boost::asio;

class EIPRRDataCmdTest : public :: testing :: Test
{

};

TEST_F(EIPRRDataCmdTest, test_serialize_mrr_data_no_data)
{
  EIP_BYTE d[8];

  EIPRRDataCmd cmd(0xA55A5AA5, 0x10, 0x73, 1, 4);
  EXPECT_EQ(sizeof(d), cmd.serializeMRRData(buffer(d)));
  EXPECT_EQ(0x10, d[0]);
  EXPECT_EQ(   3, d[1]);
  EXPECT_EQ(0x20, d[2]);
  EXPECT_EQ(0x73, d[3]);
  EXPECT_EQ(0x24, d[4]);
  EXPECT_EQ(   1, d[5]);
  EXPECT_EQ(0x30, d[6]);
  EXPECT_EQ(   4, d[7]);
}
