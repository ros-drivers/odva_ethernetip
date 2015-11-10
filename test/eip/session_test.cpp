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

using namespace eip::socket;
using namespace eip::serialization;

namespace eip {

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


TEST_F(SessionTest, test_create_connection)
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
    0x56, 0,
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
    0x1E, 0x00,
    // service code
    0xDB,
    // reserved
    0,
    // general status
    0x00,
    // size of additional status
    0,
    // response data
    0x02, 0x00, 0x0C, 0x78, 0x04, 0x00, 0x02, 0x00,
    0x89, 0x67, 0x95, 0x01, 0x21, 0x43, 0x00, 0x00,
    0xD0, 0x81, 0x17, 0x00, 0xD8, 0x5C, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x02,
    0x08, 0xAE, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x10, 0x00, 0x00, 0x02, 0x08, 0xAE, 0xC0, 0xA8,
    0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
  };

  EXPECT_EQ(0x56 + 24, sizeof(resp_packet));
  ts->rx_buffer = buffer(resp_packet);

  EIP_CONNECTION_INFO_T o_to_t, t_to_o;
  o_to_t.assembly_id = 0x71;
  o_to_t.buffer_size = 0x006E;
  o_to_t.rpi = 0x00177FA0;
  t_to_o.assembly_id = 0x66;
  t_to_o.buffer_size = 0x0584;
  t_to_o.rpi = 0x00013070;

  session.my_vendor_id_ = 0x0195;
  session.my_serial_num_ = 0x00004321;
  session.next_connection_sn_ = 0x6789;
  session.next_connection_id_ = 0x00020003;

  session.createConnection(o_to_t, t_to_o);

  // check the unregistration packet
  EXPECT_EQ(94, ts->tx_count);
  // command
  EXPECT_EQ(0x6F, ts->tx_buffer[0]);
  EXPECT_EQ(   0, ts->tx_buffer[1]);
  // length
  EXPECT_EQ(0x46, ts->tx_buffer[2]);
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
  EXPECT_EQ(0x36, ts->tx_buffer[38]);
  EXPECT_EQ(   0, ts->tx_buffer[39]);
  // service code
  EXPECT_EQ(0x5B, ts->tx_buffer[40]);
  // path
  EXPECT_EQ(0x02, ts->tx_buffer[41]);
  EXPECT_EQ(0x20, ts->tx_buffer[42]);
  EXPECT_EQ(0x06, ts->tx_buffer[43]);
  EXPECT_EQ(0x24, ts->tx_buffer[44]);
  EXPECT_EQ(0x01, ts->tx_buffer[45]);
  // forward open request
  EXPECT_EQ(0x06, ts->tx_buffer[46]);
  EXPECT_EQ(0x50, ts->tx_buffer[47]);
  EXPECT_EQ(0x03, ts->tx_buffer[48]);
  EXPECT_EQ(0x00, ts->tx_buffer[49]);
  EXPECT_EQ(0x02, ts->tx_buffer[50]);
  EXPECT_EQ(0x00, ts->tx_buffer[51]);
  EXPECT_EQ(0x04, ts->tx_buffer[52]);
  EXPECT_EQ(0x00, ts->tx_buffer[53]);
  EXPECT_EQ(0x02, ts->tx_buffer[54]);
  EXPECT_EQ(0x00, ts->tx_buffer[55]);
  EXPECT_EQ(0x89, ts->tx_buffer[56]);
  EXPECT_EQ(0x67, ts->tx_buffer[57]);
  EXPECT_EQ(0x95, ts->tx_buffer[58]);
  EXPECT_EQ(0x01, ts->tx_buffer[59]);
  EXPECT_EQ(0x21, ts->tx_buffer[60]);
  EXPECT_EQ(0x43, ts->tx_buffer[61]);
  EXPECT_EQ(0x00, ts->tx_buffer[62]);
  EXPECT_EQ(0x00, ts->tx_buffer[63]);
  EXPECT_EQ(0x00, ts->tx_buffer[64]);
  EXPECT_EQ(0x00, ts->tx_buffer[65]);
  EXPECT_EQ(0x00, ts->tx_buffer[66]);
  EXPECT_EQ(0x00, ts->tx_buffer[67]);
  EXPECT_EQ(0xA0, ts->tx_buffer[68]);
  EXPECT_EQ(0x7F, ts->tx_buffer[69]);
  EXPECT_EQ(0x17, ts->tx_buffer[70]);
  EXPECT_EQ(0x00, ts->tx_buffer[71]);
  EXPECT_EQ(0x6E, ts->tx_buffer[72]);
  EXPECT_EQ(0x00, ts->tx_buffer[73]);
  EXPECT_EQ(0x00, ts->tx_buffer[74]);
  EXPECT_EQ(0x48, ts->tx_buffer[75]);
  EXPECT_EQ(0x70, ts->tx_buffer[76]);
  EXPECT_EQ(0x30, ts->tx_buffer[77]);
  EXPECT_EQ(0x01, ts->tx_buffer[78]);
  EXPECT_EQ(0x00, ts->tx_buffer[79]);
  EXPECT_EQ(0x84, ts->tx_buffer[80]);
  EXPECT_EQ(0x05, ts->tx_buffer[81]);
  EXPECT_EQ(0x00, ts->tx_buffer[82]);
  EXPECT_EQ(0x4A, ts->tx_buffer[83]);
  EXPECT_EQ(0x01, ts->tx_buffer[84]);
  EXPECT_EQ(0x04, ts->tx_buffer[85]);
  EXPECT_EQ(0x20, ts->tx_buffer[86]);
  EXPECT_EQ(0x04, ts->tx_buffer[87]);
  EXPECT_EQ(0x24, ts->tx_buffer[88]);
  EXPECT_EQ(0x01, ts->tx_buffer[89]);
  EXPECT_EQ(0x2C, ts->tx_buffer[90]);
  EXPECT_EQ(0x71, ts->tx_buffer[91]);
  EXPECT_EQ(0x2C, ts->tx_buffer[92]);
  EXPECT_EQ(0x66, ts->tx_buffer[93]);
}

} // namespace eip
