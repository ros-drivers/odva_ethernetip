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
#include "eip/rr_data_response.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/serializable_primitive.h"

using boost::make_shared;
using namespace boost::asio;

using namespace eip;
using namespace eip::socket;
using namespace eip::serialization;

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

TEST_F(SessionTest, test_open_wrong_command_id)
{
  char resp_packet[] = {
    // command for register session
    0x00, 0x00,
    // length
    0, 0,
    // session handle
    0, 0, 0, 0,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  EXPECT_THROW(session.open("example_host"), std::runtime_error);
  EXPECT_FALSE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0, session.getSessionID());
}

TEST_F(SessionTest, test_open_zero_session)
{
  char resp_packet[] = {
    // command for register session
    0x65, 0x00,
    // length
    0, 0,
    // session handle
    0, 0, 0, 0,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  EXPECT_THROW(session.open("example_host"), std::runtime_error);
  EXPECT_FALSE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0, session.getSessionID());
}

TEST_F(SessionTest, test_open_short_packet)
{
  char resp_packet[] = {
    // command for register session
    0x65, 0x00,
    // length
    1, 0,
    // session handle
    1, 0, 0, 0,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  EXPECT_THROW(session.open("example_host"), std::runtime_error);
  EXPECT_FALSE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0, session.getSessionID());
}

TEST_F(SessionTest, test_open_short_data)
{
  char resp_packet[] = {
    // command for register session
    0x65, 0x00,
    // length
    2, 0,
    // session handle
    1, 0, 0, 0,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
    // version (short)
    1, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  session.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(1, session.getSessionID());
}

TEST_F(SessionTest, test_open_extra_data)
{
  char resp_packet[] = {
    // command for register session
    0x65, 0x00,
    // length
    8, 0,
    // session handle
    0xEF, 0xBE, 0xAD, 0xDE,
    // status
    0xFF, 0xFF, 0xFF, 0xFF,
    // sender context
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    // options
    0xFF, 0xFF, 0xFF, 0xFF,
    // protocol version
    1, 0,
    // more options flags
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  session.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0xDEADBEEF, session.getSessionID());
}

TEST_F(SessionTest, test_open_wrong_version)
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
    2, 1,
    // more options flags
    0, 0,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(resp_packet));
  Session session(ts);
  EXPECT_THROW(session.open("example_host"), std::runtime_error);
  EXPECT_FALSE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0, session.getSessionID());
}


TEST_F(SessionTest, test_get_single_attribute)
{
  char reg_resp_packet[] = {
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

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(reg_resp_packet));
  Session session(ts);
  session.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0xDEADBEEF, session.getSessionID());

  // clear out the tx buffer
  ts->tx_count = 0;
  memset(ts->tx_buffer, 0, sizeof(ts->tx_count));

  char resp_packet[] = {
    // command for register session
    0x6F, 0x00,
    // length
    24, 0,
    // session handle
    0xEF, 0xBE, 0xAD, 0xDE,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
    // interface handle
    0, 0, 0, 0,
    // timeout
    0, 0,
    // item count
    2, 0,
    // adress type
    0, 0,
    // address length
    0, 0,
    // data type
    0xB2, 0x00,
    // data length
    0x08, 0x00,
    // service code
    0x8E,
    // reserved
    0,
    // general status
    0x00,
    // size of additional status
    0,
    // response data
    0xEF, 0xCD, 0xAB, 0xAA,
  };

  ts->rx_buffer = buffer(resp_packet);

  EIP_UDINT result = session.getSingleAttribute(0x75, 1, 3, result);

  // check the unregistration packet
  EXPECT_EQ(48, ts->tx_count);
  // command
  EXPECT_EQ(0x6F, ts->tx_buffer[0]);
  EXPECT_EQ(   0, ts->tx_buffer[1]);
  // length
  EXPECT_EQ(  24, ts->tx_buffer[2]);
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
  // interface handle
  EXPECT_EQ(   0, ts->tx_buffer[24]);
  EXPECT_EQ(   0, ts->tx_buffer[25]);
  EXPECT_EQ(   0, ts->tx_buffer[26]);
  EXPECT_EQ(   0, ts->tx_buffer[27]);
  // timeout
  EXPECT_EQ(   0, ts->tx_buffer[28]);
  EXPECT_EQ(   0, ts->tx_buffer[29]);
  // item count
  EXPECT_EQ(   2, ts->tx_buffer[30]);
  EXPECT_EQ(   0, ts->tx_buffer[31]);
  // adress type null
  EXPECT_EQ(   0, ts->tx_buffer[32]);
  EXPECT_EQ(   0, ts->tx_buffer[33]);
  // address length 0
  EXPECT_EQ(   0, ts->tx_buffer[34]);
  EXPECT_EQ(   0, ts->tx_buffer[35]);
  // data type for UCMM
  EXPECT_EQ(0xB2, ts->tx_buffer[36]);
  EXPECT_EQ(0x00, ts->tx_buffer[37]);
  // data length
  EXPECT_EQ(   8, ts->tx_buffer[38]);
  EXPECT_EQ(   0, ts->tx_buffer[39]);
  // service code
  EXPECT_EQ(0xE, ts->tx_buffer[40]);
  // path
  EXPECT_EQ(0x03, ts->tx_buffer[41]);
  EXPECT_EQ(0x20, ts->tx_buffer[42]);
  EXPECT_EQ(0x75, ts->tx_buffer[43]);
  EXPECT_EQ(0x24, ts->tx_buffer[44]);
  EXPECT_EQ(0x01, ts->tx_buffer[45]);
  EXPECT_EQ(0x30, ts->tx_buffer[46]);
  EXPECT_EQ(0x03, ts->tx_buffer[47]);

  EXPECT_EQ(0xAAABCDEF, result);
}


TEST_F(SessionTest, test_set_single_attribute)
{
  char reg_resp_packet[] = {
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

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(reg_resp_packet));
  Session session(ts);
  session.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(0xDEADBEEF, session.getSessionID());

  // clear out the tx buffer
  ts->tx_count = 0;
  memset(ts->tx_buffer, 0, sizeof(ts->tx_count));

  char resp_packet[] = {
    // command for RR Data
    0x6F, 0x00,
    // length
    20, 0,
    // session handle
    0xEF, 0xBE, 0xAD, 0xDE,
    // status
    0, 0, 0, 0,
    // sender context. Should probably mirror, but this will always be 0
    0, 0, 0, 0, 0, 0, 0, 0,
    // options
    0, 0, 0, 0,
    // interface handle
    0, 0, 0, 0,
    // timeout
    0, 0,
    // item count
    2, 0,
    // adress type
    0, 0,
    // address length
    0, 0,
    // data type
    0xB2, 0x00,
    // data length
    0x04, 0x00,
    // service code
    0x90,
    // reserved
    0,
    // general status
    0x00,
    // size of additional status
    0,
  };

  ts->rx_buffer = buffer(resp_packet);

  session.setSingleAttribute(0x73, 1, 4, (EIP_UINT)1);

  // check the unregistration packet
  EXPECT_EQ(50, ts->tx_count);
  // command
  EXPECT_EQ(0x6F, ts->tx_buffer[0]);
  EXPECT_EQ(   0, ts->tx_buffer[1]);
  // length
  EXPECT_EQ(  26, ts->tx_buffer[2]);
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
  // interface handle
  EXPECT_EQ(   0, ts->tx_buffer[24]);
  EXPECT_EQ(   0, ts->tx_buffer[25]);
  EXPECT_EQ(   0, ts->tx_buffer[26]);
  EXPECT_EQ(   0, ts->tx_buffer[27]);
  // timeout
  EXPECT_EQ(   0, ts->tx_buffer[28]);
  EXPECT_EQ(   0, ts->tx_buffer[29]);
  // item count
  EXPECT_EQ(   2, ts->tx_buffer[30]);
  EXPECT_EQ(   0, ts->tx_buffer[31]);
  // adress type null
  EXPECT_EQ(   0, ts->tx_buffer[32]);
  EXPECT_EQ(   0, ts->tx_buffer[33]);
  // address length 0
  EXPECT_EQ(   0, ts->tx_buffer[34]);
  EXPECT_EQ(   0, ts->tx_buffer[35]);
  // data type for UCMM
  EXPECT_EQ(0xB2, ts->tx_buffer[36]);
  EXPECT_EQ(0x00, ts->tx_buffer[37]);
  // data length
  EXPECT_EQ(  10, ts->tx_buffer[38]);
  EXPECT_EQ(   0, ts->tx_buffer[39]);
  // service code
  EXPECT_EQ(0x10, ts->tx_buffer[40]);
  // path
  EXPECT_EQ(0x03, ts->tx_buffer[41]);
  EXPECT_EQ(0x20, ts->tx_buffer[42]);
  EXPECT_EQ(0x73, ts->tx_buffer[43]);
  EXPECT_EQ(0x24, ts->tx_buffer[44]);
  EXPECT_EQ(0x01, ts->tx_buffer[45]);
  EXPECT_EQ(0x30, ts->tx_buffer[46]);
  EXPECT_EQ(0x04, ts->tx_buffer[47]);
  // data itself
  EXPECT_EQ(0x01, ts->tx_buffer[48]);
  EXPECT_EQ(0x00, ts->tx_buffer[49]);
}
