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

#include "eip/socket/tcp_socket.h"
#include "os32c/os32c.h"
#include "os32c/range_and_reflectance_measurement.h"

using std::cout;
using std::endl;
using boost::shared_ptr;
using eip::socket::TCPSocket;
using os32c::OS32C;
using os32c::RangeAndReflectanceMeasurement;

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: os32c_node [hostname]" << endl;
    return 1;
  }
  boost::asio::io_service io_service;
  shared_ptr<TCPSocket> socket = shared_ptr<TCPSocket>(new TCPSocket(io_service));
  OS32C os32c(socket);
  try
  {
    os32c.open(argv[1]);
  }
  catch (std::runtime_error ex)
  {
    cout << "Exception caught opening session: " << ex.what() << endl;
    return -1;
  }

  try
  {
    RangeAndReflectanceMeasurement rr = os32c.getSingleRRScan();
    cout << "Received scan data" << endl;
    cout << "Scan count: " << rr.header.scan_count << endl;
    cout << "Scan period: " << rr.header.scan_rate << endl;
    cout << "Scan timestamp: " << rr.header.scan_timestamp << endl;
    cout << "Scan beam period: " << rr.header.scan_beam_period << endl;
    cout << "Range report format: " << rr.header.range_report_format << endl;
    cout << "Reflectivity report format: " << rr.header.refletivity_report_format << endl;
    cout << "Number of beams: " << rr.header.num_beams << endl;
    cout << endl << "Beam Data" << endl;
    for (int i = 0; i < rr.header.num_beams; ++i)
    {
      cout << i << ": " << rr.range_data[i] << " / " << rr.reflectance_data[i] << endl;
    }
  }
  catch (std::runtime_error ex)
  {
    cout << "Exception caught requesting scan data: " << ex.what() << endl;
  }
  catch (std::logic_error ex)
  {
    cout << "Error parsing return data: " << ex.what() << endl;
  }

  os32c.close();
  return 0;
}
