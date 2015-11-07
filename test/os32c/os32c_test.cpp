/**
Software License Agreement (proprietary)

\file      os32c_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include "os32c/os32c.h"
#include "eip/socket/test_socket.h"
#include "eip/rr_data_response.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/serializable_primitive.h"

using boost::make_shared;
using namespace boost::asio;

using namespace eip;
using namespace eip::socket;
using namespace eip::serialization;
using namespace os32c;

class OS32CTest : public :: testing :: Test
{

};

TEST_F(OS32CTest, test_calc_beam_number)
{
  EXPECT_EQ(  0, OS32C::calcBeamNumber(2.359685166149626 )); // 135.200001 degrees
  EXPECT_EQ(  0, OS32C::calcBeamNumber(2.356194491937674 )); // 135.0000001
  // EXPECT_EQ(  0, OS32C::calcBeamNumber(2.356194490192345 )); // 135
  EXPECT_EQ(  1, OS32C::calcBeamNumber(2.3561944901748917)); // 134.999999999
  EXPECT_EQ(  1, OS32C::calcBeamNumber(2.3492131733589003)); // 134.60000001
  EXPECT_EQ(338, OS32C::calcBeamNumber(0.0034906583294557337)); // 0.19999999
  EXPECT_EQ(338, OS32C::calcBeamNumber(0));
  EXPECT_EQ(338, OS32C::calcBeamNumber(-0.0034906583294557337)); // -0.19999999
  EXPECT_EQ(675, OS32C::calcBeamNumber(-2.3492131733589003)); // -134.60000001
  EXPECT_EQ(675, OS32C::calcBeamNumber(-2.3561944901748917)); // -134.999999999
  // EXPECT_EQ(676, OS32C::calcBeamNumber(-2.356194490192345 )); // -135
  EXPECT_EQ(676, OS32C::calcBeamNumber(-2.356194491937674 )); // -135.0000001
  EXPECT_EQ(676, OS32C::calcBeamNumber(-2.359685166149626 )); // -135.200001 degrees
}

TEST_F(OS32CTest, test_calc_beam_centre)
{
  EXPECT_DOUBLE_EQ(2.3596851486963333, OS32C::calcBeamCentre(0));
  EXPECT_DOUBLE_EQ(2.3527038316883564, OS32C::calcBeamCentre(1));
  EXPECT_NEAR(0, OS32C::calcBeamCentre(338), 0.00000000001);
  EXPECT_DOUBLE_EQ(-2.3527038316883564, OS32C::calcBeamCentre(675));
  EXPECT_DOUBLE_EQ(-2.3596851486963333, OS32C::calcBeamCentre(676));
}

TEST_F(OS32CTest, test_select_beams)
{
  char reg_resp_packet[] = {
    0x65, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
  };

  shared_ptr<TestSocket> ts = make_shared<TestSocket> (buffer(reg_resp_packet));
  OS32C os32c(ts);
  os32c.open("example_host");
  EXPECT_TRUE(ts->is_open);
  EXPECT_EQ("example_host", ts->hostname);
  EXPECT_EQ("44818", ts->port);
  EXPECT_EQ(5, os32c.getSessionID());

  // clear out the tx buffer
  ts->tx_count = 0;
  memset(ts->tx_buffer, 0, sizeof(ts->tx_count));

  // response packet from OS32C docs
  char resp_packet[] = {
    0x6F, 0x00, 0x14, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xB2, 0x00, 0x04, 0x00,
    0x90, 0x00, 0x00, 0x00,
  };
  ts->rx_buffer = buffer(resp_packet);

  os32c.selectBeams();

  // check the request packet
  EXPECT_EQ(136, ts->tx_count);

  EXPECT_EQ(0x6F, ts->tx_buffer[0]);
  EXPECT_EQ(0x00, ts->tx_buffer[1]);
  EXPECT_EQ(0x70, ts->tx_buffer[2]);
  EXPECT_EQ(0x00, ts->tx_buffer[3]);
  EXPECT_EQ(0x05, ts->tx_buffer[4]);
  EXPECT_EQ(0x00, ts->tx_buffer[5]);
  EXPECT_EQ(0x00, ts->tx_buffer[6]);
  EXPECT_EQ(0x00, ts->tx_buffer[7]);
  EXPECT_EQ(0x00, ts->tx_buffer[8]);
  EXPECT_EQ(0x00, ts->tx_buffer[9]);
  EXPECT_EQ(0x00, ts->tx_buffer[10]);
  EXPECT_EQ(0x00, ts->tx_buffer[11]);
  EXPECT_EQ(0x00, ts->tx_buffer[12]);
  EXPECT_EQ(0x00, ts->tx_buffer[13]);
  EXPECT_EQ(0x00, ts->tx_buffer[14]);
  EXPECT_EQ(0x00, ts->tx_buffer[15]);
  EXPECT_EQ(0x00, ts->tx_buffer[16]);
  EXPECT_EQ(0x00, ts->tx_buffer[17]);
  EXPECT_EQ(0x00, ts->tx_buffer[18]);
  EXPECT_EQ(0x00, ts->tx_buffer[19]);
  EXPECT_EQ(0x00, ts->tx_buffer[20]);
  EXPECT_EQ(0x00, ts->tx_buffer[21]);
  EXPECT_EQ(0x00, ts->tx_buffer[22]);
  EXPECT_EQ(0x00, ts->tx_buffer[23]);
  EXPECT_EQ(0x00, ts->tx_buffer[24]);
  EXPECT_EQ(0x00, ts->tx_buffer[25]);
  EXPECT_EQ(0x00, ts->tx_buffer[26]);
  EXPECT_EQ(0x00, ts->tx_buffer[27]);
  EXPECT_EQ(0x00, ts->tx_buffer[28]);
  EXPECT_EQ(0x00, ts->tx_buffer[29]);
  EXPECT_EQ(0x02, ts->tx_buffer[30]);
  EXPECT_EQ(0x00, ts->tx_buffer[31]);
  EXPECT_EQ(0x00, ts->tx_buffer[32]);
  EXPECT_EQ(0x00, ts->tx_buffer[33]);
  EXPECT_EQ(0x00, ts->tx_buffer[34]);
  EXPECT_EQ(0x00, ts->tx_buffer[35]);
  EXPECT_EQ(0xB2, ts->tx_buffer[36]);
  EXPECT_EQ(0x00, ts->tx_buffer[37]);
  EXPECT_EQ(0x60, ts->tx_buffer[38]);
  EXPECT_EQ(0x00, ts->tx_buffer[39]);
  EXPECT_EQ(0x10, ts->tx_buffer[40]);
  EXPECT_EQ(0x03, ts->tx_buffer[41]);
  EXPECT_EQ(0x20, ts->tx_buffer[42]);
  EXPECT_EQ(0x73, ts->tx_buffer[43]);
  EXPECT_EQ(0x24, ts->tx_buffer[44]);
  EXPECT_EQ(0x01, ts->tx_buffer[45]);
  EXPECT_EQ(0x30, ts->tx_buffer[46]);
  EXPECT_EQ(0x0C, ts->tx_buffer[47]);

  EXPECT_EQ(0xFF, ts->tx_buffer[48]);
  EXPECT_EQ(0xFF, ts->tx_buffer[49]);
  EXPECT_EQ(0xFF, ts->tx_buffer[50]);
  EXPECT_EQ(0xFF, ts->tx_buffer[51]);
  EXPECT_EQ(0xFF, ts->tx_buffer[52]);
  EXPECT_EQ(0xFF, ts->tx_buffer[53]);
  EXPECT_EQ(0xFF, ts->tx_buffer[54]);
  EXPECT_EQ(0xFF, ts->tx_buffer[55]);
  EXPECT_EQ(0xFF, ts->tx_buffer[56]);
  EXPECT_EQ(0xFF, ts->tx_buffer[57]);
  EXPECT_EQ(0xFF, ts->tx_buffer[58]);
  EXPECT_EQ(0xFF, ts->tx_buffer[59]);
  EXPECT_EQ(0xFF, ts->tx_buffer[60]);
  EXPECT_EQ(0xFF, ts->tx_buffer[61]);
  EXPECT_EQ(0xFF, ts->tx_buffer[62]);
  EXPECT_EQ(0xFF, ts->tx_buffer[63]);
  EXPECT_EQ(0xFF, ts->tx_buffer[64]);
  EXPECT_EQ(0xFF, ts->tx_buffer[65]);
  EXPECT_EQ(0xFF, ts->tx_buffer[66]);
  EXPECT_EQ(0xFF, ts->tx_buffer[67]);
  EXPECT_EQ(0xFF, ts->tx_buffer[68]);
  EXPECT_EQ(0xFF, ts->tx_buffer[69]);
  EXPECT_EQ(0xFF, ts->tx_buffer[70]);
  EXPECT_EQ(0xFF, ts->tx_buffer[71]);
  EXPECT_EQ(0xFF, ts->tx_buffer[72]);
  EXPECT_EQ(0xFF, ts->tx_buffer[73]);
  EXPECT_EQ(0xFF, ts->tx_buffer[74]);
  EXPECT_EQ(0xFF, ts->tx_buffer[75]);
  EXPECT_EQ(0xFF, ts->tx_buffer[76]);
  EXPECT_EQ(0xFF, ts->tx_buffer[77]);
  EXPECT_EQ(0xFF, ts->tx_buffer[78]);
  EXPECT_EQ(0xFF, ts->tx_buffer[79]);
  EXPECT_EQ(0xFF, ts->tx_buffer[80]);
  EXPECT_EQ(0xFF, ts->tx_buffer[81]);
  EXPECT_EQ(0xFF, ts->tx_buffer[82]);
  EXPECT_EQ(0xFF, ts->tx_buffer[83]);
  EXPECT_EQ(0xFF, ts->tx_buffer[84]);
  EXPECT_EQ(0xFF, ts->tx_buffer[85]);
  EXPECT_EQ(0xFF, ts->tx_buffer[86]);
  EXPECT_EQ(0xFF, ts->tx_buffer[87]);
  EXPECT_EQ(0xFF, ts->tx_buffer[88]);
  EXPECT_EQ(0xFF, ts->tx_buffer[89]);
  EXPECT_EQ(0xFF, ts->tx_buffer[90]);
  EXPECT_EQ(0xFF, ts->tx_buffer[91]);
  EXPECT_EQ(0xFF, ts->tx_buffer[92]);
  EXPECT_EQ(0xFF, ts->tx_buffer[93]);
  EXPECT_EQ(0xFF, ts->tx_buffer[94]);
  EXPECT_EQ(0xFF, ts->tx_buffer[95]);
  EXPECT_EQ(0xFF, ts->tx_buffer[96]);
  EXPECT_EQ(0xFF, ts->tx_buffer[97]);
  EXPECT_EQ(0xFF, ts->tx_buffer[98]);
  EXPECT_EQ(0xFF, ts->tx_buffer[99]);
  EXPECT_EQ(0xFF, ts->tx_buffer[100]);
  EXPECT_EQ(0xFF, ts->tx_buffer[101]);
  EXPECT_EQ(0xFF, ts->tx_buffer[102]);
  EXPECT_EQ(0xFF, ts->tx_buffer[103]);
  EXPECT_EQ(0xFF, ts->tx_buffer[104]);
  EXPECT_EQ(0xFF, ts->tx_buffer[105]);
  EXPECT_EQ(0xFF, ts->tx_buffer[106]);
  EXPECT_EQ(0xFF, ts->tx_buffer[107]);
  EXPECT_EQ(0xFF, ts->tx_buffer[108]);
  EXPECT_EQ(0xFF, ts->tx_buffer[109]);
  EXPECT_EQ(0xFF, ts->tx_buffer[110]);
  EXPECT_EQ(0xFF, ts->tx_buffer[111]);
  EXPECT_EQ(0xFF, ts->tx_buffer[112]);
  EXPECT_EQ(0xFF, ts->tx_buffer[113]);
  EXPECT_EQ(0xFF, ts->tx_buffer[114]);
  EXPECT_EQ(0xFF, ts->tx_buffer[115]);
  EXPECT_EQ(0xFF, ts->tx_buffer[116]);
  EXPECT_EQ(0xFF, ts->tx_buffer[117]);
  EXPECT_EQ(0xFF, ts->tx_buffer[118]);
  EXPECT_EQ(0xFF, ts->tx_buffer[119]);
  EXPECT_EQ(0xFF, ts->tx_buffer[120]);
  EXPECT_EQ(0xFF, ts->tx_buffer[121]);
  EXPECT_EQ(0xFF, ts->tx_buffer[122]);
  EXPECT_EQ(0xFF, ts->tx_buffer[123]);
  EXPECT_EQ(0xFF, ts->tx_buffer[124]);
  EXPECT_EQ(0xFF, ts->tx_buffer[125]);
  EXPECT_EQ(0xFF, ts->tx_buffer[126]);
  EXPECT_EQ(0xFF, ts->tx_buffer[127]);
  EXPECT_EQ(0xFF, ts->tx_buffer[128]);
  EXPECT_EQ(0xFF, ts->tx_buffer[129]);
  EXPECT_EQ(0xFF, ts->tx_buffer[130]);
  EXPECT_EQ(0xFF, ts->tx_buffer[131]);
  EXPECT_EQ(0x1F, ts->tx_buffer[132]);
  EXPECT_EQ(0x00, ts->tx_buffer[133]);
  EXPECT_EQ(0x00, ts->tx_buffer[134]);
  EXPECT_EQ(0x00, ts->tx_buffer[135]);
}
