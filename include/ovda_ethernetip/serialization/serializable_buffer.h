/**
Software License Agreement (BSD)

\file      serializable_buffer.h
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

#ifndef OVDA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_BUFFER_H
#define OVDA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_BUFFER_H

#include <boost/asio.hpp>

#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/writer.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"

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
  SerializableBuffer() : data_(NULL, 0), allocated_buffer_(NULL) { }

  /**
   * Construct a serializable buffer for the given data buffer
   * @param data Data to hold in the buffer
   */
  SerializableBuffer(mutable_buffer data) : data_(data), allocated_buffer_(NULL) { }

  virtual ~SerializableBuffer()
  {
    deleteAllocatedBuffer();
    data_ = mutable_buffer(NULL, 0);
  }

  /**
   * Get the length of the current data buffer
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
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
    // TODO: It's possible that the user could pass in the same buffer again,
    // or a subset of it. That would be absurd, but possible.
    deleteAllocatedBuffer();
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
    return writer;
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
    // with a BufferReader, we can read without copying
    BufferReader* br = dynamic_cast<BufferReader*>(&reader);
    if (br)
    {
      deleteAllocatedBuffer();
      data_ = br->readBuffer(length);
    }
    else
    {
      if (length != buffer_size(data_))
      {
        // must allocate a new buffer
        deleteAllocatedBuffer();
        allocated_buffer_ = new char[length];
        data_ = buffer(allocated_buffer_, length);
      }
      reader.readBuffer(data_);
    }
    return reader;
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
    return reader;
  }

  void operator=(mutable_buffer b)
  {
    data_ = b;
  }

private:
  mutable_buffer data_;
  char* allocated_buffer_;

  /**
   * Helper to delete an allocated buffer if needed
   */
  void deleteAllocatedBuffer()
  {
    if (allocated_buffer_)
    {
      delete[] allocated_buffer_;
      allocated_buffer_ = NULL;
    }
  }
};

} // namespace serialization
} // namespace eip
#endif  // OVDA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_BUFFER_H
