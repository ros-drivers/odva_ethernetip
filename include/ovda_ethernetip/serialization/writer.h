/**
Software License Agreement (BSD)

\file      writer.h
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

#ifndef OVDA_ETHERNETIP_SERIALIZATION_WRITER_H
#define OVDA_ETHERNETIP_SERIALIZATION_WRITER_H

#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"

using boost::asio::const_buffer;

namespace eip {
namespace serialization {

/**
 * Generic interface for writing binary data for serialization
 */
class Writer
{
public:

  /**
   * Write a value
   * @param v Value to write
   * @throw std::length_error if buffer is too small to hold the serialized value
   */
  template <typename T> void write(const T& v)
  {
    writeBytes(&v, sizeof(v));
  }

  /**
   * Write a set of bytes
   * @param p pointer to data to write
   * @param n number of bytes to write
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void writeBytes(const void* p, size_t n) = 0;

  /**
   * Write out the contents of a buffer
   * @param b buffer of data to add to the current buffer at the current location
   * @throw std::length_error if the output buffer is too small to contain the
   * contents of b
   */
  virtual void writeBuffer(const_buffer b) = 0;

  /**
   * Get the number of bytes writen to the buffer
   * @return number of bytes writen to the buffer
   */
  virtual size_t getByteCount() = 0;
};

} // namespace serialization
} // namespace eip
#endif  // OVDA_ETHERNETIP_SERIALIZATION_WRITER_H
