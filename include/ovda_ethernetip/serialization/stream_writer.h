/**
Software License Agreement (BSD)

\file      stream_writer.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_STREAM_WRITER_H
#define EIP_STREAM_WRITER_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "ovda_ethernetip/serialization/writer.h"

using boost::shared_ptr;
using boost::asio::const_buffer;
using boost::asio::buffer_cast;
using boost::asio::buffer_size;
using std::ostream;

namespace eip {
namespace serialization {

/**
 * Helper class to write values into an output stream. This is really an
 * adapter so that classes can use the Writer interface for all writing
 */
class StreamWriter : public Writer
{
public:

  /**
   * Create a writer to use the output stream given
   * @param ost Output stream to use
   */
  StreamWriter(shared_ptr<ostream> osp) : osp_(osp) { }

  /**
   * Write a value to the output stream
   * @param v Value to write to the output stream
   */
  template <typename T>
  void write(T& v)
  {
    osp_->write((char*)&v, sizeof(v));
  }

  /**
   * Write a set of bytes to the stream
   * @param p pointer to data to write
   * @param n number of bytes to write
   */
  void writeBytes(const void* p, size_t n)
  {
    osp_->write((char*)p, n);
  }

  /**
   * Write out the contents of a buffer to the stream
   * @param b buffer of data to add to the current buffer at the current location
   */
  void writeBuffer(const_buffer b)
  {
    osp_->write(buffer_cast<const char*>(b), buffer_size(b));
  }

  /**
   * Get the number of bytes writen to the stream
   * @return number of bytes writen to the stream
   */
  size_t getByteCount()
  {
    return osp_->tellp();
  }

private:
  shared_ptr<ostream> osp_;
};

} // namespace serialization
} // namespace eip
#endif  // EIP_STREAM_WRITER_H
