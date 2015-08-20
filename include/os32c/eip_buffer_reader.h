/**
Software License Agreement (proprietary)

\file      eip_buffer_reader.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_BUFFER_READER_H
#define OS32C_EIP_BUFFER_READER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using boost::asio::const_buffer;

/**
 * Helper class to read values out of a buffer to assist in deserializing.
 */
class EIPBufferReader
{
public:

  /**
   * Create a reader to use the buffer given
   * @param buf Buffer from which to read data
   */
  EIPBufferReader(const_buffer buf) : buf_(buf), byte_count_(0) { }

  /**
   * Read a value from the buffer. Automatically increases the number of 
   * bytes read and advances the pointer in the buffer.
   * @return value read from buffer of type T
   * @throw std::length_error if buffer is too small to deserialize value
   */
  template <typename T>
  void read(T& v)
  {
    if (boost::asio::buffer_size(buf_) < sizeof(T))
    {
      throw std::length_error("Buffer too small to deserialize value");
    }
    boost::asio::buffer_copy(boost::asio::buffer(&v, sizeof(v)), buf_);
    byte_count_ += sizeof(T);
    buf_ = buf_ + sizeof(T);
  }

  /**
   * Read a set of bytes from the buffer. Automatically increases the number of 
   * bytes read and advances the pointer in the buffer
   * @param n number of bytes to read
   * @return pointer into the buffer
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  const char* readBytes(size_t n)
  {
    if (boost::asio::buffer_size(buf_) < n)
    {
      throw std::length_error("Buffer too small to deserialize value");
    }
    const char* p = boost::asio::buffer_cast<const char*>(buf_);
    byte_count_ += n;
    buf_ = buf_ + n;
    return p;
  }

  /**
   * Get the number of bytes read from the buffer
   * @return number of bytes read from the buffer
   */
  size_t getByteCount()
  {
    return byte_count_;
  }

private:
  const_buffer buf_;
  size_t byte_count_;
};

#endif  // OS32C_EIP_BUFFER_READER_H
