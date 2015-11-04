/**
Software License Agreement (proprietary)

\file      session_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include "eip/session.h"
#include "eip/socket/test_socket.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"

using boost::make_shared;
using namespace boost::asio;

using namespace eip;
using namespace eip::serialization;
using namespace eip::socket;

class SessionTest : public :: testing :: Test
{

};

TEST_F(SessionTest, test_open)
{
  char resp_packet[] = {
    // command for register session
    0x65, 0x00,
    // length
    4, 0,
    // session handle
    0xEF, 0xBE, 0xAD, 0xDE,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
    // protocol version
    1, 0,
    // more options flags
    0, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  session.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);

  // check the registration packet
  EXPECT_EQ(28, ts->tx_count);
  // command
  EXPECT_EQ(0x65, ts->tx_buffer[0]);
  EXPECT_EQ(   0, ts->tx_buffer[1]);
  // length
  EXPECT_EQ(0x04, ts->tx_buffer[2]);
  EXPECT_EQ(   0, ts->tx_buffer[3]);
  // session handle
  EXPECT_EQ(   0, ts->tx_buffer[4]);
  EXPECT_EQ(   0, ts->tx_buffer[5]);
  EXPECT_EQ(   0, ts->tx_buffer[6]);
  EXPECT_EQ(   0, ts->tx_buffer[7]);
  // status
  EXPECT_EQ(   0, ts->tx_buffer[8]);
  EXPECT_EQ(   0, ts->tx_buffer[9]);
  EXPECT_EQ(   0, ts->tx_buffer[10]);
  EXPECT_EQ(   0, ts->tx_buffer[11]);
  // sender context
  EXPECT_EQ(   0, ts->tx_buffer[12]);
  EXPECT_EQ(   0, ts->tx_buffer[13]);
  EXPECT_EQ(   0, ts->tx_buffer[14]);
  EXPECT_EQ(   0, ts->tx_buffer[15]);
  EXPECT_EQ(   0, ts->tx_buffer[16]);
  EXPECT_EQ(   0, ts->tx_buffer[17]);
  EXPECT_EQ(   0, ts->tx_buffer[18]);
  EXPECT_EQ(   0, ts->tx_buffer[19]);
  // options
  EXPECT_EQ(   0, ts->tx_buffer[20]);
  EXPECT_EQ(   0, ts->tx_buffer[21]);
  EXPECT_EQ(   0, ts->tx_buffer[22]);
  EXPECT_EQ(   0, ts->tx_buffer[23]);
  // protocol version
  EXPECT_EQ(   1, ts->tx_buffer[24]);
  EXPECT_EQ(   0, ts->tx_buffer[25]);
  // more options flags
  EXPECT_EQ(   0, ts->tx_buffer[26]);
  EXPECT_EQ(   0, ts->tx_buffer[27]);

  // check that full packet used
  EXPECT_EQ(sizeof(resp_packet), ts->rx_count);

  EXPECT_EQ(0xDEADBEEF, session.getSessionID());

  // next test that close works correctly
  ts->tx_count = 0;
  memset(ts->tx_buffer, 0, sizeof(ts->tx_count));

  session.close();
  EXPECT_FALSE(ts->is_open);
  EXPECT_EQ(0, session.getSessionID());

  // check the unregistration packet
  EXPECT_EQ(24, ts->tx_count);
  // command
  EXPECT_EQ(0x66, ts->tx_buffer[0]);
  EXPECT_EQ(   0, ts->tx_buffer[1]);
  // length
  EXPECT_EQ(   0, ts->tx_buffer[2]);
  EXPECT_EQ(   0, ts->tx_buffer[3]);
  // session handle
  EXPECT_EQ(0xEF, ts->tx_buffer[4]);
  EXPECT_EQ(0xBE, ts->tx_buffer[5]);
  EXPECT_EQ(0xAD, ts->tx_buffer[6]);
  EXPECT_EQ(0xDE, ts->tx_buffer[7]);
  // status
  EXPECT_EQ(   0, ts->tx_buffer[8]);
  EXPECT_EQ(   0, ts->tx_buffer[9]);
  EXPECT_EQ(   0, ts->tx_buffer[10]);
  EXPECT_EQ(   0, ts->tx_buffer[11]);
  // sender context
  EXPECT_EQ(   0, ts->tx_buffer[12]);
  EXPECT_EQ(   0, ts->tx_buffer[13]);
  EXPECT_EQ(   0, ts->tx_buffer[14]);
  EXPECT_EQ(   0, ts->tx_buffer[15]);
  EXPECT_EQ(   0, ts->tx_buffer[16]);
  EXPECT_EQ(   0, ts->tx_buffer[17]);
  EXPECT_EQ(   0, ts->tx_buffer[18]);
  EXPECT_EQ(   0, ts->tx_buffer[19]);
  // options
  EXPECT_EQ(   0, ts->tx_buffer[20]);
  EXPECT_EQ(   0, ts->tx_buffer[21]);
  EXPECT_EQ(   0, ts->tx_buffer[22]);
  EXPECT_EQ(   0, ts->tx_buffer[23]);
}
