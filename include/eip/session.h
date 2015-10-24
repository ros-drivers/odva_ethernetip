/**
Software License Agreement (proprietary)

\file      session.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SESSION_H
#define EIP_SESSION_H

#include <string>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/serialization/serializable.h"

using std::string;
using boost::asio::ip::tcp;

namespace eip {

using eip::serialization::Serializable;

/**
 * Class to handle creating and managing Ethernet/IP Sessions
 */
class Session
{
public:

  /**
   * Construct a session to use the given io_service. Doesn't actually open
   * the port or start a session.
   */
  Session(boost::asio::io_service& io_service) : socket_(io_service), session_id_(0) { }

  virtual ~Session();

  /**
   * Open the session by opening the port and requesting a session.
   * @param hostname hostname of the target
   * @param port port to use if other than the standard
   */
  void open(string hostname, string port = "44818");

  void send(const Serializable& msg);

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

} // namespace eip

#endif  // EIP_SESSION_H
