/**
Software License Agreement (proprietary)

\file      eip_io_scanner.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_IO_SCANNER_H
#define OS32C_EIP_IO_SCANNER_H

#include <string>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using std::string;
using boost::asio::ip::udp;

/**
 * Class to scan a network for Ethernet/IP devices
 */
class EIPIOScanner
{
public:
  EIPIOScanner(boost::asio::io_service& io_service, string hostname);

  virtual ~EIPIOScanner()
  {
    socket_.close();
  }

  /**
   * Send the list identity request message
   */
  void sendListIdentityRequest();

  /**
   * Handle list identity responses
   */
  void handleListIdentityResponse(const boost::system::error_code& ec,
    std::size_t num_bytes);

  /**
   * Run the scanner indefinitely
   */
  void run();
  
private:
  string hostname_;
  udp::socket socket_;
  udp::endpoint device_endpoint_;
  boost::array<EIP_BYTE, 4*1024> recv_buf_;
};

#endif  // OS32C_EIP_IO_SCANNER_H
