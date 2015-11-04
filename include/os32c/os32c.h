/**
Software License Agreement (proprietary)

\file      os32c.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_OS32C_H
#define OS32C_OS32C_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "eip/session.h"
#include "eip/socket/socket.h"
#include "os32c/range_and_reflectance_measurement.h"

using std::vector;
using boost::shared_ptr;
using eip::Session;
using eip::socket::Socket;

namespace os32c {

/**
 * Main interface for the OS32C Laser Scanner. Produces methods to access the
 * laser scanner from a high level, including setting parameters and getting
 * single scans.
 */
class OS32C : public Session
{
public:
  /**
   * Construct a new OS32C instance.
   * @param socket Socket instance to use for communication with the lidar
   */
  OS32C(shared_ptr<Socket> socket) : Session(socket) { }

  /**
   * Make an explicit request for a single Range and Reflectance scan
   * @return Range and reflectance data received
   * @throw std::logic_error if data not received
   */
  RangeAndReflectanceMeasurement getSingleRRScan();
};

} // namespace os32c

#endif  // OS32C_OS32C_H
