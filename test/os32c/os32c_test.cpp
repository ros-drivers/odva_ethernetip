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

TEST_F(OS32CTest, test_calc_beam_mask_all)
{
  EIP_BYTE buffer[96]; // plus 32 bits on each end as guards
  memset(buffer, 0xAA, sizeof(buffer));
  EIP_BYTE* mask = buffer + 4;
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  OS32C os32c(ts);
  os32c.calcBeamMask(OS32C::ANGLE_MAX, OS32C::ANGLE_MIN, mask);
  for (size_t i = 0; i < 4; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
  for (size_t i = 0; i < 84; ++i)
  {
    EXPECT_EQ(0xFF, mask[i]);
  }
  EXPECT_EQ(0x1F, mask[84]);
  EXPECT_EQ(0, mask[85]);
  EXPECT_EQ(0, mask[86]);
  EXPECT_EQ(0, mask[87]);
  for (size_t i = 92; i < 96; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
}

TEST_F(OS32CTest, test_calc_beam_at_90)
{
  EIP_BYTE buffer[96]; // plus 32 bits on each end as guards
  memset(buffer, 0xAA, sizeof(buffer));
  EIP_BYTE* mask = buffer + 4;
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  OS32C os32c(ts);
  os32c.calcBeamMask(0.7853981633974483, -0.7853981633974483, mask);
  for (size_t i = 0; i < 4; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
  for (size_t i = 0; i < 28; ++i)
  {
    EXPECT_EQ(0, mask[i]);
  }
  EXPECT_EQ(0xFE, mask[28]);
  for (size_t i = 29; i < 56; ++i)
  {
    EXPECT_EQ(0xFF, mask[i]);
  }
  EXPECT_EQ(0x07, mask[56]);
  for (size_t i = 57; i < 88; ++i)
  {
    EXPECT_EQ(0, mask[i]);
  }
  for (size_t i = 92; i < 96; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
}

TEST_F(OS32CTest, test_calc_beam_boundaries)
{
  EIP_BYTE buffer[96]; // plus 32 bits on each end as guards
  memset(buffer, 0xAA, sizeof(buffer));
  EIP_BYTE* mask = buffer + 4;
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  OS32C os32c(ts);
  os32c.calcBeamMask(0.6911503837897546, -0.7051130178057091, mask);
  for (size_t i = 0; i < 4; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
  for (size_t i = 0; i < 29; ++i)
  {
    EXPECT_EQ(0, mask[i]);
  }
  EXPECT_EQ(0x80, mask[29]);
  for (size_t i = 30; i < 55; ++i)
  {
    EXPECT_EQ(0xFF, mask[i]);
  }
  for (size_t i = 55; i < 88; ++i)
  {
    EXPECT_EQ(0, mask[i]);
  }
  for (size_t i = 92; i < 96; ++i)
  {
    EXPECT_EQ(0xAA, buffer[i]);
  }
}

TEST_F(OS32CTest, test_calc_beam_invalid_args)
{
  EIP_BYTE buffer[96]; // plus 32 bits on each end as guards
  memset(buffer, 0xAA, sizeof(buffer));
  EIP_BYTE* mask = buffer + 4;
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  OS32C os32c(ts);
  EXPECT_THROW(os32c.calcBeamMask(2.3631758089456514, -0.7051130178057091, mask), 
    std::invalid_argument);
  EXPECT_THROW(os32c.calcBeamMask(0.6911503837897546, -2.3631758089456514, mask), 
    std::invalid_argument);
  EXPECT_THROW(os32c.calcBeamMask(0.6911503837897546, 0.6911503837897546, mask), 
    std::invalid_argument);
  EXPECT_THROW(os32c.calcBeamMask(0.6911503837897546, 0.6841690685271065, mask), 
    std::invalid_argument);
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

  os32c.selectBeams(OS32C::ANGLE_MAX, OS32C::ANGLE_MIN);

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

TEST_F(OS32CTest, test_convert_to_laserscan)
{
  shared_ptr<TestSocket> ts = make_shared<TestSocket> ();
  OS32C os32c(ts);
  os32c.setFrameID("testframe");
  RangeAndReflectanceMeasurement rr;
  rr.header.scan_count = 0xDEADBEEF;
  rr.header.scan_rate = 38609;
  rr.header.scan_timestamp = 140420933;
  rr.header.scan_beam_period = 42898;
  rr.header.machine_state = 3;
  rr.header.machine_stop_reasons = 7;
  rr.header.active_zone_set = 0;
  rr.header.zone_inputs = 0;
  rr.header.detection_zone_status = 0;
  rr.header.output_status = 0;
  rr.header.input_status = 0;
  rr.header.display_status = 0x1B1B;
  rr.header.range_report_format = RANGE_MEASURE_50M;
  rr.header.refletivity_report_format = REFLECTIVITY_MEASURE_TOT_4PS;
  rr.header.num_beams = 10;
  rr.range_data.resize(10);
  rr.range_data[0] = 1000;
  rr.range_data[1] = 1253;
  rr.range_data[2] = 1000;
  rr.range_data[3] = 1;
  rr.range_data[4] = 48750;
  rr.range_data[5] = 49999;
  rr.range_data[6] = 50001;
  rr.range_data[7] = 48135;
  rr.range_data[8] = 0xFFFF;
  rr.range_data[9] = 0xFFFF;
  rr.reflectance_data.resize(10);
  rr.reflectance_data[0] = 44000;
  rr.reflectance_data[1] = 42123;
  rr.reflectance_data[2] = 53987;
  rr.reflectance_data[3] = 0;
  rr.reflectance_data[4] = 123;
  rr.reflectance_data[5] = 555;
  rr.reflectance_data[6] = 65535;
  rr.reflectance_data[7] = 1013;
  rr.reflectance_data[8] = 0;
  rr.reflectance_data[9] = 0;

  sensor_msgs::LaserScan ls = os32c.convertToLaserScan(rr);
  EXPECT_EQ("testframe", ls.header.frame_id);
  EXPECT_FLOAT_EQ(OS32C::ANGLE_MAX, ls.angle_min);
  EXPECT_FLOAT_EQ(OS32C::ANGLE_MIN, ls.angle_max);
  EXPECT_FLOAT_EQ(OS32C::ANGLE_INC, ls.angle_increment);
  EXPECT_FLOAT_EQ(42898E-9, ls.time_increment);
  EXPECT_FLOAT_EQ( 0.002, ls.range_min);
  EXPECT_FLOAT_EQ(50.0  , ls.range_max);
  EXPECT_FLOAT_EQ( 1.0  , ls.ranges[0]);
  EXPECT_FLOAT_EQ( 1.253, ls.ranges[1]);
  EXPECT_FLOAT_EQ( 1.0  , ls.ranges[2]);
  EXPECT_FLOAT_EQ( 0.0  , ls.ranges[3]);
  EXPECT_FLOAT_EQ(48.750, ls.ranges[4]);
  EXPECT_FLOAT_EQ(49.999, ls.ranges[5]);
  EXPECT_FLOAT_EQ(50.001, ls.ranges[6]);
  EXPECT_FLOAT_EQ(48.135, ls.ranges[7]);
  EXPECT_FLOAT_EQ(50.000, ls.ranges[8]);
  EXPECT_FLOAT_EQ(50.000, ls.ranges[9]);
  EXPECT_FLOAT_EQ(44000, ls.intensities[0]);
  EXPECT_FLOAT_EQ(42123, ls.intensities[1]);
  EXPECT_FLOAT_EQ(53987, ls.intensities[2]);
  EXPECT_FLOAT_EQ(0, ls.intensities[3]);
  EXPECT_FLOAT_EQ(123, ls.intensities[4]);
  EXPECT_FLOAT_EQ(555, ls.intensities[5]);
  EXPECT_FLOAT_EQ(65535, ls.intensities[6]);
  EXPECT_FLOAT_EQ(1013, ls.intensities[7]);
  EXPECT_FLOAT_EQ(0, ls.intensities[8]);
  EXPECT_FLOAT_EQ(0, ls.intensities[9]);
}