/**
Software License Agreement (BSD)

\file      forward_open_request_test.cpp
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

#include "ovda_ethernetip/forward_open_request.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

using boost::shared_ptr;
using boost::make_shared;
using namespace boost::asio;
using eip::serialization::SerializableBuffer;
using eip::serialization::BufferReader;
using eip::serialization::BufferWriter;

namespace eip {

class ForwardOpenRequestTest : public :: testing :: Test
{

};

TEST_F(ForwardOpenRequestTest, test_calc_net_params)
{
  EXPECT_EQ(         0, ForwardOpenRequest::calcConnectionParams(0, false, 0, 0, false));
  EXPECT_EQ(0x00007FFF, ForwardOpenRequest::calcConnectionParams(0xFFFF, false, 0, 0, false));
  EXPECT_EQ(0x02000000, ForwardOpenRequest::calcConnectionParams(0, true, 0, 0, false));
  EXPECT_EQ(0x0C000000, ForwardOpenRequest::calcConnectionParams(0, false, 0xFF, 0, false));
  EXPECT_EQ(0x60000000, ForwardOpenRequest::calcConnectionParams(0, false, 0, 0xFF, false));
  EXPECT_EQ(0xCE005A5A, ForwardOpenRequest::calcConnectionParams(0x5A5A, true, CONN_PRIORITY_URGENT, CONN_TYPE_P2P, true));
}

TEST_F(ForwardOpenRequestTest, test_serialize)
{
  // 39 plus path
  EIP_BYTE d[48];

  ForwardOpenRequest data;
  data.timeout_tick_size = 6;
  data.timeout_ticks = 40;
  data.o_to_t_connection_id = 0xDEADBEEF;
  data.t_to_o_connection_id = 0xFEAD1234;
  data.connection_sn = 0xAA55;
  data.originator_vendor_id = 0xCBA0;
  data.originator_sn = 0x12345678;
  data.timeout_multiplyer = 3;
  data.o_to_t_rpi = 40000;
  data.t_to_o_rpi = 500000;
  data.setOriginToTargetParams(0x55AA, false, CONN_PRIORITY_SCHEDULED, CONN_TYPE_P2P, false);
  data.setTargetToOriginParams(0xA5A5, true, CONN_PRIORITY_SCHEDULED, CONN_TYPE_P2P, false);
  data.conn_type = 1;
  data.getPath().addLogicalClass(0x04);
  data.getPath().addLogicalInstance(1);
  data.getPath().addLogicalConnectionPoint(0x71);
  data.getPath().addLogicalConnectionPoint(0x66);

  EXPECT_EQ(sizeof(d), data.getLength());
  BufferWriter writer(buffer(d));
  data.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  // ticks
  EXPECT_EQ(   6, d[0]);
  EXPECT_EQ(  40, d[1]);
  // o to t connection ID
  EXPECT_EQ(0xEF, d[2]);
  EXPECT_EQ(0xBE, d[3]);
  EXPECT_EQ(0xAD, d[4]);
  EXPECT_EQ(0xDE, d[5]);
  // o to t connection ID
  EXPECT_EQ(0x34, d[6]);
  EXPECT_EQ(0x12, d[7]);
  EXPECT_EQ(0xAD, d[8]);
  EXPECT_EQ(0xFE, d[9]);
  // connection SN
  EXPECT_EQ(0x55, d[10]);
  EXPECT_EQ(0xAA, d[11]);
  // vendor ID
  EXPECT_EQ(0xA0, d[12]);
  EXPECT_EQ(0xCB, d[13]);
  // originator SN
  EXPECT_EQ(0x78, d[14]);
  EXPECT_EQ(0x56, d[15]);
  EXPECT_EQ(0x34, d[16]);
  EXPECT_EQ(0x12, d[17]);
  // Timeout multiplyer
  EXPECT_EQ(   3, d[18]);
  // reserved
  EXPECT_EQ(   0, d[19]);
  EXPECT_EQ(   0, d[20]);
  EXPECT_EQ(   0, d[21]);
  // O->T RPI
  EXPECT_EQ(0x40, d[22]);
  EXPECT_EQ(0x9C, d[23]);
  EXPECT_EQ(   0, d[24]);
  EXPECT_EQ(   0, d[25]);
  // O->T params
  EXPECT_EQ(0xAA, d[26]);
  EXPECT_EQ(0x55, d[27]);
  EXPECT_EQ(0x00, d[28]);
  EXPECT_EQ(0x48, d[29]);
  // T->O RPI
  EXPECT_EQ(0x20, d[30]);
  EXPECT_EQ(0xA1, d[31]);
  EXPECT_EQ(0x07, d[32]);
  EXPECT_EQ(0x00, d[33]);
  // T->O params
  EXPECT_EQ(0xA5, d[34]);
  EXPECT_EQ(0x25, d[35]);
  EXPECT_EQ(0x00, d[36]);
  EXPECT_EQ(0x4A, d[37]);
  // trigger
  EXPECT_EQ(   1, d[38]);
  // path
  EXPECT_EQ(   4, d[39]);
  EXPECT_EQ(0x20, d[40]);
  EXPECT_EQ(   4, d[41]);
  EXPECT_EQ(0x24, d[42]);
  EXPECT_EQ(   1, d[43]);
  EXPECT_EQ(0x2C, d[44]);
  EXPECT_EQ(0x71, d[45]);
  EXPECT_EQ(0x2C, d[46]);
  EXPECT_EQ(0x66, d[47]);
}

} // namespace eip
