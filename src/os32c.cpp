/**
Software License Agreement (proprietary)

\file      os32c.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <ros/ros.h>

#include "os32c/os32c.h"

using std::cout;
using std::endl;
using eip::Session;
using eip::RRDataResponse;
using os32c::RangeAndReflectanceMeasurement;

namespace os32c {

RangeAndReflectanceMeasurement OS32C::getSingleRRScan()
{
  RRDataResponse resp = getSingleAttribute(0x75, 1, 3);
  cout << "Got RRData Response" << endl;
  cout << "Service code: " << (int)resp.getServiceCode() << endl;
  cout << "General status: " << (int)resp.getGeneralStatus() << endl;
  if (resp.getAdditionalStatus())
  {
    cout << "WARNING: Got additional status length: " << resp.getAdditionalStatus()->getLength() << endl;
  }
  if (resp.getResponseData())
  {
    RangeAndReflectanceMeasurement rr;
    resp.getResponseDataAs(rr);
    return rr;
  }
  else
  {
    throw std::logic_error("No response data received");
  }
}

} // namespace os32c
