/**
Software License Agreement (proprietary)

\file      range_and_reflectance_measurement_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/range_and_reflectance_measurement.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"

using namespace boost::asio;
using namespace os32c;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class RangeAndReflectanceMeasurementTest : public :: testing :: Test
{

};

TEST_F(RangeAndReflectanceMeasurementTest, test_deserialize)
{
  EIP_BYTE d[56 + 4000];

  // use a measurement report header to serialize the header data
  MeasurementReportHeader mrh;
  mrh.scan_count = 0xDEADBEEF;
  mrh.scan_rate = 40000;
  mrh.scan_timestamp = 0x55AA55AA;
  mrh.scan_beam_period = 43333;
  mrh.machine_state = 3;
  mrh.machine_stop_reasons = 7;
  mrh.active_zone_set = 0x45;
  mrh.zone_inputs = 0xAA;
  mrh.detection_zone_status = 0x0F;
  mrh.output_status = 7;
  mrh.input_status = 3;
  mrh.display_status = 0x0402;
  mrh.non_safety_config_checksum = 0x55AA;
  mrh.safety_config_checksum = 0x5AA5;
  mrh.range_report_format = 1;
  mrh.refletivity_report_format = 1;
  mrh.num_beams = 1000;

  BufferWriter writer(buffer(d));
  mrh.serialize(writer);

  for (EIP_UINT i = 10000; i < 10000 + 1000; ++i) {
    writer.write(i);
  }

  for (EIP_UINT i = 20000; i < 20000 + 1000; ++i) {
    writer.write(i);
  }

  ASSERT_EQ(sizeof(d), writer.getByteCount());

  BufferReader reader(buffer(d));
  RangeAndReflectanceMeasurement mr;
  mr.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());

  EXPECT_EQ(0xDEADBEEF, mr.header.scan_count);
  EXPECT_EQ(40000, mr.header.scan_rate);
  EXPECT_EQ(0x55AA55AA, mr.header.scan_timestamp);
  EXPECT_EQ(43333, mr.header.scan_beam_period);
  EXPECT_EQ(3, mr.header.machine_state);
  EXPECT_EQ(7, mr.header.machine_stop_reasons);
  EXPECT_EQ(0x45, mr.header.active_zone_set);
  EXPECT_EQ(0xAA, mr.header.zone_inputs);
  EXPECT_EQ(0x0F, mr.header.detection_zone_status);
  EXPECT_EQ(7, mr.header.output_status);
  EXPECT_EQ(3, mr.header.input_status);
  EXPECT_EQ(0x0402, mr.header.display_status);
  EXPECT_EQ(0x55AA, mr.header.non_safety_config_checksum);
  EXPECT_EQ(0x5AA5, mr.header.safety_config_checksum);
  EXPECT_EQ(1, mr.header.range_report_format);
  EXPECT_EQ(1, mr.header.refletivity_report_format);
  EXPECT_EQ(1000, mr.header.num_beams);

  EXPECT_EQ(1000, mr.range_data.size());
  for (int i = 0; i < mr.range_data.size(); ++i)
  {
    EXPECT_EQ(i + 10000, mr.range_data[i]);
  }

  EXPECT_EQ(1000, mr.reflectance_data.size());
  for (int i = 0; i < mr.reflectance_data.size(); ++i)
  {
    EXPECT_EQ(i + 20000, mr.reflectance_data[i]);
  }
}

TEST_F(RangeAndReflectanceMeasurementTest, test_serialize)
{
  RangeAndReflectanceMeasurement mr;
  mr.header.scan_count = 0xDEADBEEF;
  mr.header.scan_rate = 40000;
  mr.header.scan_timestamp = 0x55AA55AA;
  mr.header.scan_beam_period = 43333;
  mr.header.machine_state = 3;
  mr.header.machine_stop_reasons = 7;
  mr.header.active_zone_set = 0x45;
  mr.header.zone_inputs = 0xAA;
  mr.header.detection_zone_status = 0x0F;
  mr.header.output_status = 7;
  mr.header.input_status = 3;
  mr.header.display_status = 0x0402;
  mr.header.non_safety_config_checksum = 0x55AA;
  mr.header.safety_config_checksum = 0x5AA5;
  mr.header.range_report_format = 1;
  mr.header.refletivity_report_format = 1;
  mr.header.num_beams = 1000;

  mr.range_data.resize(1000);
  for (int i = 0; i <  1000; ++i) {
    mr.range_data[i] = i + 30000;
  }

  mr.reflectance_data.resize(1000);
  for (int i = 0; i <  1000; ++i) {
    mr.reflectance_data[i] = i + 40000;
  }

  EIP_BYTE d[56 + 4000];
  EXPECT_EQ(sizeof(d), mr.getLength());
  BufferWriter writer(buffer(d));
  mr.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  EXPECT_EQ(d[0], 0xEF);
  EXPECT_EQ(d[1], 0xBE);
  EXPECT_EQ(d[2], 0xAD);
  EXPECT_EQ(d[3], 0xDE);
  EXPECT_EQ(d[4], 0x40);
  EXPECT_EQ(d[5], 0x9C);
  EXPECT_EQ(d[6], 0);
  EXPECT_EQ(d[7], 0);
  EXPECT_EQ(d[8], 0xAA);
  EXPECT_EQ(d[9], 0x55);
  EXPECT_EQ(d[10], 0xAA);
  EXPECT_EQ(d[11], 0x55);
  EXPECT_EQ(d[12], 0x45);
  EXPECT_EQ(d[13], 0xA9);
  EXPECT_EQ(d[14], 0);
  EXPECT_EQ(d[15], 0);
  EXPECT_EQ(d[16], 3);
  EXPECT_EQ(d[17], 0);
  EXPECT_EQ(d[18], 7);
  EXPECT_EQ(d[19], 0);
  EXPECT_EQ(d[20], 0x45);
  EXPECT_EQ(d[21], 0);
  EXPECT_EQ(d[22], 0xAA);
  EXPECT_EQ(d[23], 0);
  EXPECT_EQ(d[24], 0x0F);
  EXPECT_EQ(d[25], 0);
  EXPECT_EQ(d[26], 0x07);
  EXPECT_EQ(d[27], 0);
  EXPECT_EQ(d[28], 0x03);
  EXPECT_EQ(d[29], 0);
  EXPECT_EQ(d[30], 0x02);
  EXPECT_EQ(d[31], 0x04);
  EXPECT_EQ(d[32], 0xAA);
  EXPECT_EQ(d[33], 0x55);
  EXPECT_EQ(d[34], 0xA5);
  EXPECT_EQ(d[35], 0x5A);
  EXPECT_EQ(d[36], 0);
  EXPECT_EQ(d[37], 0);
  EXPECT_EQ(d[38], 0);
  EXPECT_EQ(d[39], 0);
  EXPECT_EQ(d[40], 0);
  EXPECT_EQ(d[41], 0);
  EXPECT_EQ(d[42], 0);
  EXPECT_EQ(d[43], 0);
  EXPECT_EQ(d[44], 0);
  EXPECT_EQ(d[45], 0);
  EXPECT_EQ(d[46], 0);
  EXPECT_EQ(d[47], 0);
  EXPECT_EQ(d[48], 1);
  EXPECT_EQ(d[49], 0);
  EXPECT_EQ(d[50], 1);
  EXPECT_EQ(d[51], 0);
  EXPECT_EQ(d[52], 0);
  EXPECT_EQ(d[53], 0);
  EXPECT_EQ(d[54], 0xE8);
  EXPECT_EQ(d[55], 0x03);

  for (int i = 0; i < 1000; ++i)
  {
    EIP_UINT exp_value = i + 30000;
    EXPECT_EQ((exp_value) & 0x00FF, d[56 + i*2]);
    EXPECT_EQ((exp_value >> 8) & 0x00FF, d[56 + i*2 + 1]);
  }

  for (int i = 0; i < 1000; ++i)
  {
    EIP_UINT exp_value = i + 40000;
    EXPECT_EQ((exp_value) & 0x00FF, d[56 + 2000 + i*2]);
    EXPECT_EQ((exp_value >> 8) & 0x00FF, d[56 + 2000 + i*2 + 1]);
  }
}
