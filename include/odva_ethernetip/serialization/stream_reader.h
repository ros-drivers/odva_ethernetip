/**
Software License Agreement (BSD)

\file      stream_reader.h
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

#ifndef ODVA_ETHERNETIP_SERIALIZATION_STREAM_READER_H
#define ODVA_ETHERNETIP_SERIALIZATION_STREAM_READER_H

#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "odva_ethernetip/serialization/reader.h"

using boost::shared_ptr;
using boost::asio::mutable_buffer;
using boost::asio::buffer_cast;
using boost::asio::buffer_size;
using std::istream;

namespace eip {
namespace serialization {

/**
 * Helper class to read values out of a stream to assist in deserializing.
 * Really just an adapter so that classes can use the Reader interface for all
 * deserialization tasks.
 */
class StreamReader : public Reader
{
public:

  /**
   * Create a reader to use the input stream given
   * @param isp Input stream to use for inputs
   */
  StreamReader(shared_ptr<istream> isp) : isp_(isp) { }

  /**
   * Read a set of bytes from the reader into a given buffer
   * @param buf Buffer into which to copy bytes
   * @param n number of bytes to read
   */
  void readBytes(void* buf, size_t n)
  {
    isp_->read((char*)buf, n);
  }

  /**
   * Read a set of bytes into the given buffer
   * @param buf Buffer to use for reading
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  void readBuffer(mutable_buffer buf)
  {
    readBytes(buffer_cast<void*>(buf), buffer_size(buf));
  }

  /**
   * Get the number of bytes read from the buffer
   * @return number of bytes read from the buffer
   */
  size_t getByteCount()
  {
    return isp_->tellg();
  }

  /**
   * Skip forward in input by n bytes
   * @param n number of bytes to skip forward
   */
  virtual void skip(size_t n)
  {
    isp_->seekg(n, std::ios_base::cur);
  }
private:
  shared_ptr<istream> isp_;
};

} // namespace serialization
} // namespace eip
#endif  // ODVA_ETHERNETIP_SERIALIZATION_STREAM_READER_H
