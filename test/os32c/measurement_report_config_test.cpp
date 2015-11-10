/**
Software License Agreement (proprietary)

\file      measurement_report_config_test.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "os32c/measurement_report_config.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/serialization/buffer_reader.h"

using namespace boost::asio;
using namespace os32c;
using namespace eip;
using namespace eip::serialization;
using namespace boost::asio;

class MeasurementReportConfigTest : public :: testing :: Test
{

};

TEST_F(MeasurementReportConfigTest, test_deserialize)
{
  EIP_BYTE d[] = {
    0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
    0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  BufferReader reader(buffer(d));
  MeasurementReportConfig mrc;
  mrc.deserialize(reader);
  EXPECT_EQ(sizeof(d), reader.getByteCount());
  EXPECT_EQ(sizeof(d), mrc.getLength());
  EXPECT_EQ(0x0001, mrc.sequence_num);
  EXPECT_EQ(0x00000003, mrc.trigger);
  EXPECT_EQ(1, mrc.range_report_format);
  EXPECT_EQ(2, mrc.reflectivity_report_format);
  for (size_t i = 0; i < 41; ++i)
  {
    EXPECT_EQ(0, mrc.beam_selection_mask[i]);
  }
  EXPECT_EQ(0xFF, mrc.beam_selection_mask[41]);
  EXPECT_EQ(0xFF, mrc.beam_selection_mask[42]);
  EXPECT_EQ(0x0F, mrc.beam_selection_mask[43]);
  for (size_t i = 44; i < 88; ++i)
  {
    EXPECT_EQ(0, mrc.beam_selection_mask[i]);
  }
}

TEST_F(MeasurementReportConfigTest, test_serialize)
{
  MeasurementReportConfig mrc;
  mrc.sequence_num = 0x0001;
  mrc.trigger = 0x00000003;
  mrc.range_report_format = 1;
  mrc.reflectivity_report_format = 2;
  memset(mrc.beam_selection_mask, 0, sizeof(mrc.beam_selection_mask));
  mrc.beam_selection_mask[41] = 0xFF;
  mrc.beam_selection_mask[42] = 0xFF;
  mrc.beam_selection_mask[43] = 0x0F;

  EIP_BYTE d[110];
  EXPECT_EQ(sizeof(d), mrc.getLength());
  BufferWriter writer(buffer(d));
  mrc.serialize(writer);
  EXPECT_EQ(sizeof(d), writer.getByteCount());

  EXPECT_EQ(0x01, d[0]);
  EXPECT_EQ(0x00, d[1]);
  EXPECT_EQ(0x03, d[2]);
  EXPECT_EQ(0x00, d[3]);
  EXPECT_EQ(0x00, d[4]);
  EXPECT_EQ(0x00, d[5]);
  EXPECT_EQ(0x01, d[6]);
  EXPECT_EQ(0x00, d[7]);
  EXPECT_EQ(0x02, d[8]);
  EXPECT_EQ(0x00, d[9]);
  EXPECT_EQ(0x00, d[10]);
  EXPECT_EQ(0x00, d[11]);
  EXPECT_EQ(0x00, d[12]);
  EXPECT_EQ(0x00, d[13]);
  EXPECT_EQ(0x00, d[14]);
  EXPECT_EQ(0x00, d[15]);
  EXPECT_EQ(0x00, d[16]);
  EXPECT_EQ(0x00, d[17]);
  EXPECT_EQ(0x00, d[18]);
  EXPECT_EQ(0x00, d[19]);
  EXPECT_EQ(0x00, d[20]);
  EXPECT_EQ(0x00, d[21]);
  EXPECT_EQ(0x00, d[22]);
  EXPECT_EQ(0x00, d[23]);
  EXPECT_EQ(0x00, d[24]);
  EXPECT_EQ(0x00, d[25]);
  EXPECT_EQ(0x00, d[26]);
  EXPECT_EQ(0x00, d[27]);
  EXPECT_EQ(0x00, d[28]);
  EXPECT_EQ(0x00, d[29]);
  EXPECT_EQ(0x00, d[30]);
  EXPECT_EQ(0x00, d[31]);
  EXPECT_EQ(0x00, d[32]);
  EXPECT_EQ(0x00, d[33]);
  EXPECT_EQ(0x00, d[34]);
  EXPECT_EQ(0x00, d[35]);
  EXPECT_EQ(0x00, d[36]);
  EXPECT_EQ(0x00, d[37]);
  EXPECT_EQ(0x00, d[38]);
  EXPECT_EQ(0x00, d[39]);
  EXPECT_EQ(0x00, d[40]);
  EXPECT_EQ(0x00, d[41]);
  EXPECT_EQ(0x00, d[42]);
  EXPECT_EQ(0x00, d[43]);
  EXPECT_EQ(0x00, d[44]);
  EXPECT_EQ(0x00, d[45]);
  EXPECT_EQ(0x00, d[46]);
  EXPECT_EQ(0x00, d[47]);
  EXPECT_EQ(0x00, d[48]);
  EXPECT_EQ(0x00, d[49]);
  EXPECT_EQ(0x00, d[50]);
  EXPECT_EQ(0x00, d[51]);
  EXPECT_EQ(0x00, d[52]);
  EXPECT_EQ(0x00, d[53]);
  EXPECT_EQ(0x00, d[54]);
  EXPECT_EQ(0x00, d[55]);
  EXPECT_EQ(0x00, d[56]);
  EXPECT_EQ(0x00, d[57]);
  EXPECT_EQ(0x00, d[58]);
  EXPECT_EQ(0x00, d[59]);
  EXPECT_EQ(0x00, d[60]);
  EXPECT_EQ(0x00, d[61]);
  EXPECT_EQ(0x00, d[62]);
  EXPECT_EQ(0xFF, d[63]);
  EXPECT_EQ(0xFF, d[64]);
  EXPECT_EQ(0x0F, d[65]);
  EXPECT_EQ(0x00, d[66]);
  EXPECT_EQ(0x00, d[67]);
  EXPECT_EQ(0x00, d[68]);
  EXPECT_EQ(0x00, d[69]);
  EXPECT_EQ(0x00, d[70]);
  EXPECT_EQ(0x00, d[71]);
  EXPECT_EQ(0x00, d[72]);
  EXPECT_EQ(0x00, d[73]);
  EXPECT_EQ(0x00, d[74]);
  EXPECT_EQ(0x00, d[75]);
  EXPECT_EQ(0x00, d[76]);
  EXPECT_EQ(0x00, d[77]);
  EXPECT_EQ(0x00, d[78]);
  EXPECT_EQ(0x00, d[79]);
  EXPECT_EQ(0x00, d[80]);
  EXPECT_EQ(0x00, d[81]);
  EXPECT_EQ(0x00, d[82]);
  EXPECT_EQ(0x00, d[83]);
  EXPECT_EQ(0x00, d[84]);
  EXPECT_EQ(0x00, d[85]);
  EXPECT_EQ(0x00, d[86]);
  EXPECT_EQ(0x00, d[87]);
  EXPECT_EQ(0x00, d[88]);
  EXPECT_EQ(0x00, d[89]);
  EXPECT_EQ(0x00, d[90]);
  EXPECT_EQ(0x00, d[91]);
  EXPECT_EQ(0x00, d[92]);
  EXPECT_EQ(0x00, d[93]);
  EXPECT_EQ(0x00, d[94]);
  EXPECT_EQ(0x00, d[95]);
  EXPECT_EQ(0x00, d[96]);
  EXPECT_EQ(0x00, d[97]);
  EXPECT_EQ(0x00, d[98]);
  EXPECT_EQ(0x00, d[99]);
  EXPECT_EQ(0x00, d[100]);
  EXPECT_EQ(0x00, d[101]);
  EXPECT_EQ(0x00, d[102]);
  EXPECT_EQ(0x00, d[103]);
  EXPECT_EQ(0x00, d[104]);
  EXPECT_EQ(0x00, d[105]);
  EXPECT_EQ(0x00, d[106]);
  EXPECT_EQ(0x00, d[107]);
  EXPECT_EQ(0x00, d[108]);
  EXPECT_EQ(0x00, d[109]);
}
