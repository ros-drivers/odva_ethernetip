/**
Software License Agreement (proprietary)

\file      socket.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SOCKET_SOCKET_H
#define EIP_SOCKET_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "eip/serialization/serializable.h"
#include "eip/serialization/buffer_writer.h"

using std::string;
using std::size_t;
using boost::asio::buffer;
using boost::asio::const_buffer;
using boost::asio::mutable_buffer;

namespace eip {

using serialization::Serializable;
using serialization::BufferWriter;

namespace socket {

/**
 * Generic interface for sockets. Provides the ability to produce testable
 * implementations that don't rely on actual sockets.
 */
class Socket
{
public:

  virtual ~Socket() { }

  /**
   * Open the socket to connect to the given hostname and port
   * @param hostname Hostname to connect to
   * @param port Name or number of port to connect to
   */
  virtual void open(string hostname, string port) = 0;

  /**
   * Close the socket
   */
  virtual void close() = 0;

  /**
   * Send the contents of the buffer out the socket
   * @param buf Data to send out the socket
   * @return number of bytes written
   */
  virtual size_t send(const_buffer buf) = 0;

  /**
   * Send the serializable message out the socket
   * @param msg Serializable message to send
   * @return number of bytes sent
   */
  virtual size_t send(const Serializable& msg)
  {
    std::vector<char> buf(msg.getLength());
    BufferWriter writer(buffer(buf));
    msg.serialize(writer);
    return send(buffer(buf));
  }

  /**
   * Receive bytes from the socket
   * @param buf Buffer into which to store received data
   * @return number of bytes read
   */
  virtual size_t receive(mutable_buffer buf) = 0;

};

} // namespace socket
} // namespace eip
#endif  // EIP_SOCKET_SOCKET_H
