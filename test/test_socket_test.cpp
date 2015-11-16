/**
Software License Agreement (BSD)

\file      test_socket_test.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
