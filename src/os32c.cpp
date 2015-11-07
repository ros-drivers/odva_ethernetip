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

void OS32C::selectBeams()
{
  // enable all beams
  EIP_WORD beams[44];
  memset(beams, 0xFF, sizeof(beams));
  beams[43] = 0;
  // only turn on the first 5 bits of the last word
  beams[42] = 0x1F;
  shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>(buffer(beams));
  setSingleAttributeSerializable(0x73, 1, 12, sb);
}

RangeAndReflectanceMeasurement OS32C::getSingleRRScan()
{
  RangeAndReflectanceMeasurement rr;
  getSingleAttributeSerializable(0x75, 1, 3, rr);
  return rr;
}

} // namespace os32c
