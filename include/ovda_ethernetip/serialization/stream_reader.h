/**
Software License Agreement (BSD)

\file      stream_reader.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_STREAM_READER_H
#define EIP_STREAM_READER_H

#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/serialization/reader.h"

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
#endif  // EIP_STREAM_READER_H
