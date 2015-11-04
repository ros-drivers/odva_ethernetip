/**
Software License Agreement (proprietary)

\file      test_socket.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SOCKET_TEST_SOCKET_H
#define EIP_SOCKET_TEST_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "eip/socket/socket.h"

using std::size_t;
using std::string;
using namespace boost::asio;

namespace eip {
namespace socket {

/**
 * Fake socket for testing
 */
class TestSocket : public Socket
{
public:

  /// Whether the socket has been opened
  bool is_open;

  /// Hostname given at open
  string hostname;

  /// Port given at open
  string port;

  /// Characters passed in during send()
  char tx_buffer[4096];
  /// Number of characters sent via send()
  size_t tx_count;

  /// Characters to return on calls to receive()
  const_buffer rx_buffer;
  /// Number of characters requested by client by calling receive()
  size_t rx_count;

  TestSocket() : tx_count(0), rx_count(0), is_open(false) { }

  TestSocket(const_buffer rbuf) : tx_count(0), rx_count(0), rx_buffer(rbuf), is_open(false) { }

  /**
   * Open the socket to connect to the given hostname and port
   * @param hostname Hostname to connect to
   * @param port Name or number of port to connect to
   */
  virtual void open(string hostname, string port)
  {
    this->hostname = hostname;
    this->port = port;
    is_open = true;
  }

  /**
   * Close the socket
   */
  virtual void close()
  {
    is_open = false;
  }

  /**
   * Send the contents of the buffer out the socket
   * @param buf Data to send out the socket
   * @return number of bytes written
   */
  virtual size_t send(const_buffer buf)
  {
    size_t n = buffer_copy(buffer(tx_buffer) + tx_count, buf);
    tx_count += n;
    return n;
  }

  /**
   * Receive bytes from the socket
   * @param buf Buffer into which to store received data
   * @return number of bytes read
   */
  virtual size_t receive(mutable_buffer buf)
  {
    size_t n = buffer_copy(buf, rx_buffer);
    rx_count += n;
    rx_buffer = rx_buffer + n;
    return n;
  }

};

} // namespace socket
} // namespace eip
#endif  // EIP_SOCKET_TEST_SOCKET_H
