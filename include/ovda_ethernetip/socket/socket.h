/**
Software License Agreement (BSD)

\file      socket.h
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

#ifndef OVDA_ETHERNETIP_SOCKET_SOCKET_H
#define OVDA_ETHERNETIP_SOCKET_SOCKET_H

#include <string>
#include <boost/asio.hpp>

#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

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
#endif  // OVDA_ETHERNETIP_SOCKET_SOCKET_H
