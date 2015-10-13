/**
Software License Agreement (proprietary)

\file      buffer_reader.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_BUFFER_READER_H
#define EIP_BUFFER_READER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"

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

private:
  mutable_buffer buf_;
  size_t byte_count_;
};

} // namespace serialization
} // namespace eip
#endif  // EIP_BUFFER_READER_H
