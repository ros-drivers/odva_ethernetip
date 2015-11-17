/**
Software License Agreement (BSD)

\file      buffer_reader.h
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

#ifndef OVDA_ETHERNETIP_SERIALIZATION_BUFFER_READER_H
#define OVDA_ETHERNETIP_SERIALIZATION_BUFFER_READER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/reader.h"

using boost::asio::buffer;
using boost::asio::mutable_buffer;
using boost::asio::buffer_size;

namespace eip {
namespace serialization {

/**
 * Helper class to read values out of a buffer to assist in deserializing.
 */
class BufferReader : public Reader
{
public:

  /**
   * Create a reader to use the buffer given
   * @param buf Buffer from which to read data
   */
  BufferReader(mutable_buffer buf) : buf_(buf), byte_count_(0) { }

  /**
   * Copy a set of bytes from the buffer into a given buffer.
   * @param buf Buffer into which to copy bytes
   * @param n Number of bytes to copy
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void readBytes(void* buf, size_t n)
  {
    readBuffer(buffer(buf, n));
  }

  /**
   * Read a set of bytes from the input buffer without copying
   * @param n Number of bytes to read
   * @return pointer into the output buffer
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void* readBytes(size_t n)
  {
    return boost::asio::buffer_cast<void*>(readBuffer(n));
  }

  /**
   * Read a set of bytes from the buffer without copying.
   * @param n number of bytes to read
   * @return buffer of resulting data
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual mutable_buffer readBuffer(size_t n)
  {
    if (buffer_size(buf_) < n)
    {
      throw std::length_error("Buffer too small to deserialize value");
    }
    mutable_buffer b = buffer(buf_, n);
    byte_count_ += n;
    buf_ = buf_ + n;
    return b;
  }

  /**
   * Read a set of bytes from the input buffer and copy into the given buffer.
   * Automatically increases the number of bytes read and advances the pointer
   * in the buffer
   * @param buf Buffer into which to copy data
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void readBuffer(mutable_buffer buf)
  {
    size_t n = buffer_size(buf);
    if (buffer_size(buf_) < n)
    {
      throw std::length_error("Buffer too small to deserialize value");
    }
    boost::asio::buffer_copy(buf, buf_);
    byte_count_ += n;
    buf_ = buf_ + n;
  }

  /**
   * Get the number of bytes read from the buffer
   * @return number of bytes read from the buffer
   */
  virtual size_t getByteCount()
  {
    return byte_count_;
  }

  /**
   * Skip forward in input by n bytes
   * @param n number of bytes to skip forward
   * @throw std::length_error if end of output buffer reached
   */
  virtual void skip(size_t n)
  {
    if (buffer_size(buf_) < n)
    {
      throw std::length_error("End of buffer reached skipping forward");
    }
    byte_count_ += n;
    buf_ = buf_ + n;
  }
private:
  mutable_buffer buf_;
  size_t byte_count_;
};

} // namespace serialization
} // namespace eip
#endif  // OVDA_ETHERNETIP_SERIALIZATION_BUFFER_READER_H
