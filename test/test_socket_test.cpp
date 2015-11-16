/**
Software License Agreement (proprietary)

\file      test_socket_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/socket/test_socket.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using namespace eip::socket;

class TestSocketTest : public :: testing :: Test
{

};

TEST_F(TestSocketTest, test_open)
{
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  shared_ptr<Socket> s = ts;
  s->open("lehost", "aport");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("lehost", ts->hostname);
  EXPECT_EQ("aport", ts->port);
  s->close();
  EXPECT_FALSE(ts->is_open);
}

TEST_F(TestSocketTest, test_send)
{
  unsigned char d[] = { 0xAA, 0x55, 0x34, 0x12, 0xEF, 0xBE, 0xAD, 0xDE };
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  shared_ptr<Socket> s = ts;

  s->send(buffer(d, 4));
  EXPECT_EQ(4, ts->tx_count);
  s->send(buffer(d));
  EXPECT_EQ(12, ts->tx_count);
  EXPECT_EQ(0xAA, ts->tx_buffer[0]);
  EXPECT_EQ(0x55, ts->tx_buffer[1]);
  EXPECT_EQ(0x34, ts->tx_buffer[2]);
  EXPECT_EQ(0x12, ts->tx_buffer[3]);
  EXPECT_EQ(0xAA, ts->tx_buffer[4]);
  EXPECT_EQ(0x55, ts->tx_buffer[5]);
}

TEST_F(TestSocketTest, test_receive)
{
  unsigned char input[] = { 0xAA, 0x55, 0x34, 0x12, 0xEF, 0xBE, 0xAD, 0xDE };
  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(input));
  shared_ptr<Socket> s = ts;

  unsigned char r[2];
  size_t n = s->receive(buffer(r));
  EXPECT_EQ(2, n);
  EXPECT_EQ(0xAA, r[0]);
  EXPECT_EQ(0x55, r[1]);

  unsigned char r2[8];
  n = s->receive(buffer(r2));
  EXPECT_EQ(6, n);
  EXPECT_EQ(0x34, r2[0]);
  EXPECT_EQ(0x12, r2[1]);
  EXPECT_EQ(0xEF, r2[2]);
  EXPECT_EQ(0xBE, r2[3]);
  EXPECT_EQ(0xAD, r2[4]);
  EXPECT_EQ(0xDE, r2[5]);
}
