/**
Software License Agreement (BSD)

\file      eip_path_test.cpp
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
#include <boost/asio.hpp>

#include "ovda_ethernetip/path.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

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

TEST_F(PathTest, test_connection)
{
  EIP_BYTE d[9];

  Path p;
  p.addLogicalClass(4);
  p.addLogicalInstance(1);
  p.addLogicalConnectionPoint(0x71);
  p.addLogicalConnectionPoint(0x66);
  EXPECT_EQ(sizeof(d), p.getLength());
  BufferWriter writer(buffer(d));
  p.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());
  EXPECT_EQ(   4, d[0]);
  EXPECT_EQ(0x20, d[1]);
  EXPECT_EQ(   4, d[2]);
  EXPECT_EQ(0x24, d[3]);
  EXPECT_EQ(   1, d[4]);
  EXPECT_EQ(0x2C, d[5]);
  EXPECT_EQ(0x71, d[6]);
  EXPECT_EQ(0x2C, d[7]);
  EXPECT_EQ(0x66, d[8]);
}
