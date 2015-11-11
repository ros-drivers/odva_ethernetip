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

#include <gtest/gtest_prod.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <sensor_msgs/LaserScan.h>

#include "eip/session.h"
#include "eip/socket/socket.h"
#include "os32c/measurement_report.h"
#include "os32c/measurement_report_config.h"
#include "os32c/range_and_reflectance_measurement.h"

using std::vector;
using boost::shared_ptr;
using sensor_msgs::LaserScan;
using eip::Session;
using eip::socket::Socket;

#define DEG2RAD(a) (a * M_PI / 180)
#define RAD2DEG(a) (a * 180 / M_PI)

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
  OS32C(shared_ptr<Socket> socket, shared_ptr<Socket> io_socket) 
    : Session(socket, io_socket), frame_id_("OS32C"),
      start_angle_(ANGLE_MAX), end_angle_(ANGLE_MIN),
      connection_num_(-1), mrc_sequence_num_(1) { }

  static const double ANGLE_MIN = DEG2RAD(-135.2);
  static const double ANGLE_MAX = DEG2RAD( 135.2);
  static const double ANGLE_INC = DEG2RAD(0.4);
  static const double DISTANCE_MIN = 0.002;
  static const double DISTANCE_MAX = 50;

  /**
   * Get the frame ID to be sent with laser scans
   * @return Current frame ID
   */
  const string& getFrameID() const
  {
    return frame_id_;
  }

  /**
   * Set the frame ID to be sent with laser scans
   * @param frame_id Frame ID to send
   */
  void setFrameID(const string& frame_id)
  {
    frame_id_ = frame_id;
  }

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
   * Select which beams are to be measured. Must be set before requesting
   * measurements. Angles are in ROS conventions. Zero is straight ahead.
   * Positive numbers are CCW and all numbers are in radians.
   * @param start_angle Start angle in ROS conventions
   * @param end_angle End angle in ROS conventions
   */
  void selectBeams(double start_angle, double end_angle);

  /**
   * Make an explicit request for a single Range and Reflectance scan
   * @return Range and reflectance data received
   * @throw std::logic_error if data not received
   */
  RangeAndReflectanceMeasurement getSingleRRScan();

  /**
   * Calculate the beam number on the lidar for a given ROS angle. Note that
   * in ROS angles are given as radians CCW with zero being straight ahead,
   * While the lidar start its scan at the most CCW position and moves positive
   * CW, with zero being at halfway through the scan.
   * Based on my calculations and the OS32C docs, there are 677 beams and the scan
   * area is 135.4 to -135.4 degrees, with a 0.4 degree pitch. The beam centres
   * must then be at 135.2, 134.8, ... 0.4, 0, 0.4, ... -134.8, -135.2.
   * @param angle Radians CCW from straight ahead
   * @return OS32C beam number
   */
  static inline int calcBeamNumber(double angle)
  {
    return (ANGLE_MAX - angle + ANGLE_INC / 2) / ANGLE_INC;
  }

  /**
   * Calculate the ROS angle for a beam given the OS32C beam number
   * @param beam_num Beam number, starting with 0 being the most CCW beam and 
   *  positive moving CW around the scan
   * @return ROS Angle
   */
  static inline double calcBeamCentre(int beam_num)
  {
    return ANGLE_MAX - beam_num * ANGLE_INC;
  }

  /**
   * Helper to convert a Range and Reflectance Measurement to a ROS LaserScan
   * @param rr Measurement to convert
   * @return ROS LaserScan
   */
  LaserScan convertToLaserScan(const RangeAndReflectanceMeasurement& rr);

  /**
   * Helper to convert a Measurement Report to a ROS LaserScan
   * @param mr Measurement to convert
   * @return ROS LaserScan
   */
  LaserScan convertToLaserScan(const MeasurementReport& mr);

  void sendMeasurmentReportConfigUDP();

  MeasurementReport receiveMeasurementReportUDP();

  void startUDPIO();

private:
  // allow unit tests to access the helpers below for direct testing
  FRIEND_TEST(OS32CTest, test_calc_beam_mask_all);
  FRIEND_TEST(OS32CTest, test_calc_beam_at_90);
  FRIEND_TEST(OS32CTest, test_calc_beam_boundaries);
  FRIEND_TEST(OS32CTest, test_calc_beam_invalid_args);
  FRIEND_TEST(OS32CTest, test_convert_to_laserscan);

  double start_angle_;
  double end_angle_;
  string frame_id_;

  // data for sending to lidar to keep UDP session alive
  int connection_num_;
  MeasurementReportConfig mrc_;
  EIP_UDINT mrc_sequence_num_;

  /**
   * Helper to calculate the mask for a given start and end beam angle
   * @param start_angle Angle of the first beam in the scan
   * @param end_angle Angle of the last beam in the scan
   * @param mask Holder for the mask data. Must be 88 bytes
   */
  void calcBeamMask(double start_angle, double end_angle, EIP_BYTE mask[]);
};

} // namespace os32c

#endif  // OS32C_OS32C_H
