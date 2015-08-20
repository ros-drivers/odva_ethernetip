/**
Software License Agreement (proprietary)

\file      eip_buffer_writer.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_BUFFER_WRITER_H
#define OS32C_EIP_BUFFER_WRITER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using boost::asio::mutable_buffer;

/**
 * Helper class to write values into a buffer to assist in serializing.
 */
class EIPBufferWriter
{
public:

  /**
   * Create a writer to use the buffer given
   * @param buf Buffer to which to write data
   */
  EIPBufferWriter(mutable_buffer buf) : buf_(buf), byte_count_(0) { }

  /**
   * Write a value to the buffer. Automatically increases the number of 
   * bytes write and advances the pointer in the buffer.
   * @return value write from buffer of type T
   * @throw std::length_error if buffer is too small to hold the serialized value
   */
  template <typename T>
  void write(T& v)
  {
    if (boost::asio::buffer_size(buf_) < sizeof(T))
    {
      throw std::length_error("Buffer too small to serialize value");
    }
    boost::asio::buffer_copy(buf_, boost::asio::buffer(&v, sizeof(v)));
    byte_count_ += sizeof(T);
    buf_ = buf_ + sizeof(T);
  }

  /**
   * Write a set of bytes to the buffer. Automatically increases the number of 
   * bytes writen and advances the pointer in the buffer
   * @param p pointer to data to write
   * @param n number of bytes to write
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  void writeBytes(const void* p, size_t n)
  {
    if (boost::asio::buffer_size(buf_) < n)
    {
      throw std::length_error("Buffer too small to deserialize value");
    }
    boost::asio::buffer_copy(buf_, boost::asio::buffer(p, n));
    byte_count_ += n;
    buf_ = buf_ + n;
  }

  /**
   * Get the number of bytes writen to the buffer
   * @return number of bytes writen to the buffer
   */
  size_t getByteCount()
  {
    return byte_count_;
  }

private:
  mutable_buffer buf_;
  size_t byte_count_;
};

#endif  // OS32C_EIP_BUFFER_WRITER_H
