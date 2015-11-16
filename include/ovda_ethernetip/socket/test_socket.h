/**
Software License Agreement (BSD)

\file      test_socket.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OVDA_ETHERNETIP_SOCKET_TEST_SOCKET_H
#define OVDA_ETHERNETIP_SOCKET_TEST_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "ovda_ethernetip/socket/socket.h"

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
  unsigned char tx_buffer[4096];
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

  /**
   * Helper to clear out the TX buffer between tests
   */
  void clearTxBuffer()
  {
    tx_count = 0;
    memset(tx_buffer, 0, sizeof(tx_count));
  }

};

} // namespace socket
} // namespace eip
#endif  // OVDA_ETHERNETIP_SOCKET_TEST_SOCKET_H
