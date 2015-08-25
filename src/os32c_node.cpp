/**
Software License Agreement (proprietary)

\file      os32c_node.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <ros/ros.h>

#include "os32c/eip_session.h"
#include "os32c/eip_io_scanner.h"

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: os32c_node [hostname]" << endl;
    return 1;
  }
  boost::asio::io_service io_service;
  try
  {
    EIPSession session(io_service);
    session.open(argv[1]);
  }
  catch (std::runtime_error ex)
  {
    cout << "Exception caught opening session: " << ex.what() << endl;
  }
  EIPIOScanner scanner(io_service, argv[1]);
  scanner.run();
  return 0;
}