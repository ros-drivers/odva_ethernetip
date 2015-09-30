/**
Software License Agreement (proprietary)

\file      eip_cpf_item_primitive_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include "os32c/eip_cpf_item_primitive.h"

class EIPCPFItemPrimitiveTest : public :: testing :: Test
{

};

TEST_F(EIPCPFItemPrimitiveTest, test_int_value)
{
  EIPCPFItemPrimitive<EIP_UDINT> item(0x55AA, 0xDEADBEEF);
  EXPECT_EQ(0x55AA, item.getItemType());
  EXPECT_EQ(4, item.getDataLength());
  EXPECT_EQ(0xDEADBEEF, item.getItemData());
}

TEST_F(EIPCPFItemPrimitiveTest, test_serialize_int)
{
  EIPCPFItemPrimitive<EIP_UDINT> item(0xAA55, 0xDEADBEEF);

  EIP_BYTE d[40];
  boost::iostreams::basic_array_sink<char> sr((char*)d, sizeof(d));  
  boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > os(sr);
  os << item;

  EXPECT_EQ(0x55, d[0]);
  EXPECT_EQ(0xAA, d[1]);
  EXPECT_EQ(0x04, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0xEF, d[4]);
  EXPECT_EQ(0xBE, d[5]);
  EXPECT_EQ(0xAD, d[6]);
  EXPECT_EQ(0xDE, d[7]);
}
