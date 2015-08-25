/**
Software License Agreement (proprietary)

\file      eip_session.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_SESSION_H
#define OS32C_EIP_SESSION_H

#include <string>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using std::string;
using boost::asio::ip::tcp;

/**
 * Class to handle creating and managing Ethernet/IP Sessions
 */
class EIPSession
{
public:

  /**
   * Construct a session to use the given io_service. Doesn't actually open
   * the port or start a session.
   */
  EIPSession(boost::asio::io_service& io_service);

  /**
   * Open the session by opening the port and requesting a session.
   * @param hostname hostname of the target
   * @param port port to use if other than the standard
   */
  void open(string hostname, string port = "44818");

  /**
   * Close the session by unregistering the session and then closing the port
   */
  void close();

  /**
   * Get the ID number assigned to this session by the target
   * @return session ID number
   */
  EIP_UINT getSessionID()
  {
    return session_id_;
  }
private:
  tcp::socket socket_;
  EIP_UINT session_id_;
};

#endif  // OS32C_EIP_SESSION_H
