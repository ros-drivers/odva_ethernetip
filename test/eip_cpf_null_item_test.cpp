/**
Software License Agreement (proprietary)

\file      eip_cpf_null_item_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include "os32c/eip_cpf_null_item.h"

class EIPCPFNullItemTest : public :: testing :: Test
{

};

TEST_F(EIPCPFNullItemTest, test_construct)
{
  EIPCPFNullItem item;
  EXPECT_EQ(0, item.getItemType());
  EXPECT_EQ(0, item.getDataLength());
}

TEST_F(EIPCPFNullItemTest, test_serialize_int)
{
  unsigned char d[4];
  boost::iostreams::basic_array_sink<char> sr((char*)d, sizeof(d));  
  boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > os(sr);

  EIPCPFNullItem item;
  os << item;
  EXPECT_EQ(0, d[0]);
  EXPECT_EQ(0, d[1]);
  EXPECT_EQ(0, d[2]);
  EXPECT_EQ(0, d[3]);
}
