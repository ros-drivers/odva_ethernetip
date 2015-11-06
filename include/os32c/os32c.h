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


typedef enum
{
  NO_TOF_MEASUREMENTS       = 0,
  RANGE_MEASURE_50M         = 1,
  RANGE_MEASURE_32M_PZ      = 2,
  RANGE_MEASURE_16M_WZ1PZ   = 3,
  RANGE_MEASURE_8M_WZ2WZ1PZ = 4,
  RANGE_MEASURE_TOF_4PS     = 5,
} OS32C_RANGE_FORMAT;

typedef enum
{
  NO_TOT_MEASUREMENTS               = 0,
  REFLECTIVITY_MEASURE_TOT_ENCODED  = 1,
  REFLECTIVITY_MEASURE_TOT_4PS      = 2,
} OS32C_REFLECTIVITY_FORMAT;

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
   * Get the range format code. Does a Get Single Attribute to the scanner
   * to get the current range format.
   * @return range format code.
   * @see OS32C_RANGE_FORMAT
   */
  EIP_UINT getRangeFormat();

  /**
   * Set the range format code for the scanner. Does a Set Single Attribute to
   * the scanner to set the range format.
   * @param format The range format code to set
   * @see OS32C_REFLECTIVITY_FORMAT
   */
  void setRangeFormat(EIP_UINT format);

  /**
   * Get the reflectivity format code. Does a Get Single Attribute to the scanner
   * to get the current reflectivity format.
   * @return reflectivity format code.
   * @see OS32C_REFLECTIVITY_FORMAT
   */
  EIP_UINT getReflectivityFormat();

  /**
   * Set the reflectivity format code for the scanner. Does a Set Single Attribute to
   * the scanner to set the reflectivity format.
   * @param format The reflectivity format code to set
   * @see OS32C_RANGE_FORMAT
   */
  void setReflectivityFormat(EIP_UINT format);


  /**
   * Currently just selects all beams. Will be changed in the future to provide
   * ability to select which beams are being used
   */
  void selectBeams();

  /**
   * Make an explicit request for a single Range and Reflectance scan
   * @return Range and reflectance data received
   * @throw std::logic_error if data not received
   */
  RangeAndReflectanceMeasurement getSingleRRScan();
};

} // namespace os32c

#endif  // OS32C_OS32C_H
