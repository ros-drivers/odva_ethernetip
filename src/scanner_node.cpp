/**
Software License Agreement (proprietary)

\file      scanner_node.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <ros/ros.h>

#include "eip/io_scanner.h"
#include "eip/io_scanner.h"

using std::cout;
using std::endl;
using eip::IOScanner;

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
    IOScanner scanner(io_service, argv[1]);
    scanner.run();
  }
  catch (std::runtime_error ex)
  {
    cout << "Exception caught doing IO scan: " << ex.what() << endl;
  }
  return 0;
}