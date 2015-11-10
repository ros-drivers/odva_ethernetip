/**
Software License Agreement (proprietary)

\file      os32c_node.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <ros/ros.h>
#include <boost/shared_ptr.hpp>
#include <sensor_msgs/LaserScan.h>

#include "eip/socket/tcp_socket.h"
#include "os32c/os32c.h"
#include "os32c/range_and_reflectance_measurement.h"

using std::cout;
using std::endl;
using boost::shared_ptr;
using sensor_msgs::LaserScan;
using eip::socket::TCPSocket;
using namespace os32c;

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "os32c");
  ros::NodeHandle nh;

  // get sensor config from params
  string hostname, frame_id;
  double start_angle, end_angle;
  ros::param::param<std::string>("~hostname", hostname, "192.168.1.1");
  ros::param::param<std::string>("~frame_id", frame_id, "OS32C");
  ros::param::param<double>("~start_angle", start_angle, OS32C::ANGLE_MAX);
  ros::param::param<double>("~end_angle", end_angle, OS32C::ANGLE_MIN);

  // publisher for laserscans
  ros::Publisher laserscan_pub = nh.advertise<LaserScan>("laser", 1);

  boost::asio::io_service io_service;
  shared_ptr<TCPSocket> socket = shared_ptr<TCPSocket>(new TCPSocket(io_service));
  // TODO: this should be a UDP socket directed at the hostname
  shared_ptr<TCPSocket> io_socket = shared_ptr<TCPSocket>(new TCPSocket(io_service));
  OS32C os32c(socket, io_socket);

  try
  {
    os32c.open(hostname);
  }
  catch (std::runtime_error ex)
  {
    cout << "Exception caught opening session: " << ex.what() << endl;
    return -1;
  }

  try
  {
    os32c.setRangeFormat(RANGE_MEASURE_50M);
    os32c.setReflectivityFormat(REFLECTIVITY_MEASURE_TOT_4PS);
    os32c.selectBeams(start_angle, end_angle);
  }
  catch (std::invalid_argument ex)
  {
    cout << "Invalid arguments in sensor configuration: " << ex.what() << endl;
    return -1;
  }

  while (ros::ok())
  {
    try
    {
      RangeAndReflectanceMeasurement rr = os32c.getSingleRRScan();
      LaserScan ls = os32c.convertToLaserScan(rr);
      laserscan_pub.publish(ls);
    }
    catch (std::runtime_error ex)
    {
      cout << "Exception caught requesting scan data: " << ex.what() << endl;
    }
    catch (std::logic_error ex)
    {
      cout << "Error parsing return data: " << ex.what() << endl;
    }
  }

  os32c.close();
  return 0;
}
