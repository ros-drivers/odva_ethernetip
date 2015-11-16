/**
Software License Agreement (BSD)

\file      udp_socket.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SOCKET_UDP_SOCKET_H
#define EIP_SOCKET_UDP_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "ovda_ethernetip/socket/socket.h"

using std::size_t;
using std::string;
using namespace boost::asio;
using boost::asio::ip::udp;

namespace eip {
namespace socket {

/**
 * TCP Implementation of the Socket interface
 */
class UDPSocket : public Socket
{
public:

  UDPSocket(io_service& io_serv, unsigned short local_port)
    : socket_(io_serv), local_endpoint_(udp::v4(), local_port) { }

  /**
   * Open the socket to connect to the given hostname and port
   * @param hostname Hostname to connect to
   * @param port Name or number of port to connect to
   */
  virtual void open(string hostname, string port)
  {
    udp::resolver resolver(socket_.get_io_service());
    udp::resolver::query query(udp::v4(), hostname, port);
    remote_endpoint_ = *resolver.resolve(query);
    socket_.open(udp::v4());
    socket_.bind(local_endpoint_);
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
    return socket_.send_to(const_buffers_1(buf), remote_endpoint_);
  }

  /**
   * Receive bytes from the socket
   * @param buf Buffer into which to store received data
   * @return number of bytes read
   */
  virtual size_t receive(mutable_buffer buf)
  {
    // have to make this into a sequence or the templating engine throws a fit
    udp::endpoint ep;
    return socket_.receive_from(mutable_buffers_1(buf), ep);
  }

private:
  udp::socket socket_;
  udp::endpoint local_endpoint_;
  udp::endpoint remote_endpoint_;
};

} // namespace socket
} // namespace eip
#endif  // EIP_SOCKET_UDP_SOCKET_H
