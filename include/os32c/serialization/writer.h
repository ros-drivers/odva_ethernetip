/**
Software License Agreement (proprietary)

\file      writer.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_WRITER_H
#define EIP_WRITER_H

#include <boost/asio.hpp>

#include "os32c/eip_types.h"

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

  /**
   * Write using the << operator
   * @param v Value to write
   */
  template <typename T> Writer& operator<<(const T& v)
  {
    write(v);
  }
};

} // namespace serialization
} // namespace eip
#endif  // EIP_WRITER_H
