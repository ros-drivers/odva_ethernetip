/**
Software License Agreement (proprietary)

\file      serializable_buffer.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SERIALIZABLE_BUFFER_H
#define EIP_SERIALIZABLE_BUFFER_H

#include <boost/asio.hpp>

#include "os32c/serialization/serializable.h"
#include "os32c/serialization/writer.h"
#include "os32c/serialization/reader.h"
#include "os32c/serialization/buffer_reader.h"

using boost::asio::mutable_buffer;
using boost::asio::buffer_size;

namespace eip {
namespace serialization {

/**
 * Generic buffer of data that can be serialized
 */
class SerializableBuffer : public Serializable
{
public:

  /**
   * Construct an empty serializable buffer
   * @param data Data to hold in the buffer
   */
  SerializableBuffer() : data_(NULL, 0) { }

  /**
   * Construct a serializable buffer for the given data buffer
   * @param data Data to hold in the buffer
   */
  SerializableBuffer(mutable_buffer data) : data_(data) { }

  /**
   * Get the length of the current data buffer
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength()
  {
    return buffer_size(data_);
  }

  /**
   * Get the data held in this buffer
   * @return current data buffer
   */
  virtual mutable_buffer getData() const
  {
    return data_;
  }

  /**
   * Set the data for the buffer
   * @param data Data to assign to the buffer
   */
  virtual void setData(mutable_buffer data)
  {
    data_ = data;
  }

  /**
   * Serialize the buffer into the writer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.writeBuffer(data_);
  }

  /**
   * Deserialize buffer data from the given reader, up to the length given.
   * Will use the buffer already allocated.
   * @param reader Reader to use for deserialization
   * @param length Length expected for data
   * @return the reader again
   * @throw std::length_error if the given length is greater than allocated 
   *   data buffer
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    if (length > buffer_size(data_))
    {
      throw std::length_error("Given data length longer than data buffer");
    }
    reader.readBuffer(buffer(data_, length));
  }

  /**
   * Deserialize data into this buffer, using the size of the buffer already
   * allocated
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    reader.readBuffer(data_);
  }

  /**
   * Deserialize from a BufferReader without copying. Asks the BufferReader
   * for length bytes out of the existing buffer.
   * @param reader BufferReader to use for reading
   * @param length the number of bytes to request
   * @return the BufferReader again
   * @throw std::length_error if the length requested is greater than the
   *   remaining bytes in the reader's buffer.
   */
  virtual BufferReader& deserialize(BufferReader& reader, size_t length)
  {
    data_ = reader.readBuffer(length);
  }

  void operator=(mutable_buffer b)
  {
    data_ = b;
  }

private:
  mutable_buffer data_;
};

} // namespace serialization
} // namespace eip
#endif  // EIP_SERIALIZABLE_BUFFER_H
