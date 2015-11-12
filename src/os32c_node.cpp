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
#include "eip/socket/udp_socket.h"
#include "os32c/os32c.h"
#include "os32c/range_and_reflectance_measurement.h"

using std::cout;
using std::endl;
using boost::shared_ptr;
using sensor_msgs::LaserScan;
using eip::socket::TCPSocket;
using eip::socket::UDPSocket;
using namespace os32c;

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "os32c");
  ros::NodeHandle nh;

  // get sensor config from params
  string host, frame_id;
  double start_angle, end_angle;
  ros::param::param<std::string>("~host", host, "192.168.1.1");
  ros::param::param<std::string>("~frame_id", frame_id, "laser");
  ros::param::param<double>("~start_angle", start_angle, OS32C::ANGLE_MAX);
  ros::param::param<double>("~end_angle", end_angle, OS32C::ANGLE_MIN);

  // publisher for laserscans
  ros::Publisher laserscan_pub = nh.advertise<LaserScan>("scan", 1);

  boost::asio::io_service io_service;
  shared_ptr<TCPSocket> socket = shared_ptr<TCPSocket>(new TCPSocket(io_service));
  shared_ptr<UDPSocket> io_socket = shared_ptr<UDPSocket>(new UDPSocket(io_service, 2222));
  OS32C os32c(socket, io_socket);

  try
  {
    os32c.open(host);
  }
  catch (std::runtime_error ex)
  {
    ROS_FATAL_STREAM("Exception caught opening session: " << ex.what());
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
    ROS_FATAL_STREAM("Invalid arguments in sensor configuration: " << ex.what());
    return -1;
  }

  try
  {
    os32c.startUDPIO();
    os32c.sendMeasurmentReportConfigUDP();
  }
  catch (std::logic_error ex)
  {
    ROS_FATAL_STREAM("Could not start UDP IO: " << ex.what());
    return -1;
  }

  int ctr = 10;
  sensor_msgs::LaserScan laserscan_msg;
  os32c.fillLaserScanStaticConfig(&laserscan_msg);
  laserscan_msg.header.frame_id = frame_id;

  while (ros::ok())
  {
    try
    {
      // Collect measurement from device, convert to ROS message format.
      MeasurementReport report = os32c.receiveMeasurementReportUDP();
      OS32C::convertToLaserScan(report, &laserscan_msg);

      // Stamp and publish message.
      laserscan_msg.header.stamp = ros::Time::now();
      laserscan_msg.header.seq++;
      laserscan_pub.publish(laserscan_msg);

      // Every tenth message received, send the keepalive message in response.
      // TODO: Make this time-based instead of message-count based.
      if (++ctr > 10)
      {
        os32c.sendMeasurmentReportConfigUDP();
        ctr = 0;
      }
    }
    catch (std::runtime_error ex)
    {
      ROS_ERROR_STREAM("Exception caught requesting scan data: " << ex.what());
    }
    catch (std::logic_error ex)
    {
      ROS_ERROR_STREAM("Problem parsing return data: " << ex.what());
    }

    ros::spinOnce();
  }

  os32c.closeConnection(0);
  os32c.close();
  return 0;
}
