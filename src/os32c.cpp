/**
Software License Agreement (proprietary)

\file      os32c.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <ros/ros.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "os32c/os32c.h"
#include "eip/serialization/serializable_buffer.h"

using std::cout;
using std::endl;
using boost::shared_ptr;
using boost::make_shared;
using boost::asio::buffer;
using eip::Session;
using eip::serialization::SerializableBuffer;
using eip::RRDataResponse;
using os32c::RangeAndReflectanceMeasurement;

namespace os32c {

const double OS32C::ANGLE_MIN;
const double OS32C::ANGLE_MAX;
const double OS32C::ANGLE_INC;
const double OS32C::DISTANCE_MIN;
const double OS32C::DISTANCE_MAX;

EIP_UINT OS32C::getRangeFormat()
{
  return getSingleAttribute(0x73, 1, 4, (EIP_UINT)0);
}

void OS32C::setRangeFormat(EIP_UINT format)
{
  setSingleAttribute(0x73, 1, 4, format);
}

EIP_UINT OS32C::getReflectivityFormat()
{
  return getSingleAttribute(0x73, 1, 5, (EIP_UINT)0);
}

void OS32C::setReflectivityFormat(EIP_UINT format)
{
  setSingleAttribute(0x73, 1, 5, format);
}

void OS32C::calcBeamMask(double start_angle, double end_angle, EIP_BYTE mask[])
{
  if (start_angle > (ANGLE_MAX + ANGLE_INC / 2))
  {
    throw std::invalid_argument("Start angle is greater than max");
  }
  if (end_angle < (ANGLE_MIN - ANGLE_INC / 2))
  {
    throw std::invalid_argument("End angle is greater than max");
  }
  if (start_angle - end_angle <= ANGLE_INC)
  {
    throw std::invalid_argument("Starting angle is less than ending angle");
  }

  int start_beam = calcBeamNumber(start_angle);
  int end_beam = calcBeamNumber(end_angle);
  start_angle_ = calcBeamCentre(start_beam);
  end_angle_ = calcBeamCentre(end_beam);

  // figure out where we're starting and ending in the array
  int start_byte = start_beam / 8;
  int start_bit = start_beam - start_byte * 8;
  int end_byte = end_beam / 8;
  int end_bit = end_beam - end_byte * 8;

  // clear out all of the whole bytes that need to be set in one shot
  if (start_byte > 0)
  {
    memset(mask, 0, start_byte);
  }

  // if we're starting in the middle of the byte, then we need to do some bit math
  if (start_bit)
  {
    // clear out the bits at the start of the byte, while making the higher bits 1
    mask[start_byte] = ~((1 << start_bit) - 1);
  }
  else
  {
    --start_byte;
  }

  // mark all of the whole bytes
  memset(mask + start_byte + 1, 0xFF, end_byte - start_byte - 1);

  // clear out the bits above the end beam
  mask[end_byte] = (1 << end_bit + 1) - 1;

  // zero out everything after the end
  memset(mask + end_byte + 1, 0, 87 - end_byte);
}

void OS32C::selectBeams(double start_angle, double end_angle)
{
  EIP_BYTE beams[88];
  calcBeamMask(start_angle, end_angle, beams);
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(beams));
  setSingleAttributeSerializable(0x73, 1, 12, sb);
}

RangeAndReflectanceMeasurement OS32C::getSingleRRScan()
{
  RangeAndReflectanceMeasurement rr;
  getSingleAttributeSerializable(0x75, 1, 3, rr);
  return rr;
}

LaserScan OS32C::convertToLaserScan(const RangeAndReflectanceMeasurement& rr)
{
  if (rr.range_data.size() != rr.header.num_beams ||
    rr.reflectance_data.size() != rr.header.num_beams)
  {
    throw std::invalid_argument("Number of beams does not match vector size");
  }

  LaserScan ls;
  ls.header.seq = rr.header.scan_count;
  ls.header.frame_id = frame_id_;
  // TODO: figure out time stamp conversion
  // ls.header.stamp = rr.header.scan_timestamp;
  // TODO: seq number?
  ls.angle_min = start_angle_;
  ls.angle_max = end_angle_;
  ls.angle_increment = ANGLE_INC;
  // beam period is in ns
  ls.time_increment = rr.header.scan_beam_period / 1000000000.0;
  // scan rate (more correctly, scan period) is in us
  ls.scan_time = rr.header.scan_rate / 1000000.0;
  ls.range_min = DISTANCE_MIN;
  ls.range_max = DISTANCE_MAX;

  // TODO: this currently makes assumptions of the report format. Should likely
  // accomodate all of them, or at least anything reasonable.
  ls.ranges.resize(rr.header.num_beams);
  ls.intensities.resize(rr.header.num_beams);
  for (int i = 0; i < rr.header.num_beams; ++i)
  {
    if (rr.range_data[i] == 0x0001)
    {
      // noisy beam detected
      ls.ranges[i] = 0;
    }
    else if (rr.range_data[i] == 0xFFFF)
    {
      // no return
      ls.ranges[i] = DISTANCE_MAX;
    }
    else
    {
      ls.ranges[i] = rr.range_data[i] / 1000.0;
    }
    ls.intensities[i] = rr.reflectance_data[i];
  }
  return ls;
}

} // namespace os32c
