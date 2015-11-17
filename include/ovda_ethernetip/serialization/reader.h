/**
Software License Agreement (BSD)

\file      reader.h
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

#ifndef OVDA_ETHERNETIP_SERIALIZATION_READER_H
#define OVDA_ETHERNETIP_SERIALIZATION_READER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"

using boost::asio::mutable_buffer;

namespace eip {
namespace serialization {

/**
 * Generic interface for reading binary data
 */
class Reader
{
public:

  /**
   * Read a value
   * @return value read from input of type T
   * @throw std::length_error if buffer is too small to deserialize value
   */
  template <typename T> void read(T& v)
  {
    readBytes(&v, sizeof(v));
  }

  /**
   * Read a set of bytes from the input into the given buffer
   * @param buf buffer to use for reading
   * @param n number of bytes to read
   * @return pointer into the buffer
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void readBytes(void* buf, size_t n) = 0;

  /**
   * Read input into the given buffer
   * @param buf Buffer into which to copy input
   */
  virtual void readBuffer(mutable_buffer buf) = 0;

  /**
   * Get the number of bytes read from the buffer
   * @return number of bytes read from the buffer
   */
  virtual size_t getByteCount() = 0;

  /**
   * Skip forward in input by n bytes
   * @param n number of bytes to skip forward
   * @throw std::length_error if end of output buffer reached
   */
  virtual void skip(size_t n) = 0;
};

} // namespace serialization
} // namespace eip
#endif  // OVDA_ETHERNETIP_SERIALIZATION_READER_H
