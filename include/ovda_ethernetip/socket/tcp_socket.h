/**
Software License Agreement (proprietary)

\file      tcp_socket.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SOCKET_TCP_SOCKET_H
#define EIP_SOCKET_TCP_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "ovda_ethernetip/socket/socket.h"

using std::size_t;
using std::string;
using namespace boost::asio;
using boost::asio::ip::tcp;

namespace eip {
namespace socket {

/**
 * TCP Implementation of the Socket interface
 */
class TCPSocket : public Socket
{
public:

  TCPSocket(io_service& io_serv) : socket_(io_serv) { }

  /**
   * Open the socket to connect to the given hostname and port
   * @param hostname Hostname to connect to
   * @param port Name or number of port to connect to
   */
  virtual void open(string hostname, string port)
  {
    tcp::resolver resolver(socket_.get_io_service());
    tcp::resolver::query query(hostname, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    connect(socket_, endpoint_iterator);
  }

  /**
   * Close the socket
   */
  virtual inline void close()
  {
    socket_.close();
  }

  /**
   * Send the contents of the buffer out the socket
   * @param buf Data to send out the socket
   * @return number of bytes written
   */
  virtual size_t send(const_buffer buf)
  {
    return socket_.send(const_buffers_1(buf));
  }

  /**
   * Receive bytes from the socket
   * @param buf Buffer into which to store received data
   * @return number of bytes read
   */
  virtual size_t receive(mutable_buffer buf)
  {
    // have to make this into a sequence or the templating engine throws a fit
    return socket_.receive(mutable_buffers_1(buf));
  }

private:
  tcp::socket socket_;
};

} // namespace socket
} // namespace eip
#endif  // EIP_SOCKET_TCP_SOCKET_H
