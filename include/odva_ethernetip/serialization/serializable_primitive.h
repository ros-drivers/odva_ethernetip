/**
Software License Agreement (BSD)

\file      serializable_primitive.h
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

#ifndef ODVA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_PRIMITIVE_H
#define ODVA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_PRIMITIVE_H

#include "odva_ethernetip/serialization/serializable.h"
#include "odva_ethernetip/serialization/writer.h"
#include "odva_ethernetip/serialization/reader.h"

namespace eip {
namespace serialization {

/**
 * Serialization of a generic primitive class that can be serialized as just the
 * in memory data as is.
 */
template <typename T>
class SerializablePrimitive : public Serializable
{
public:

  /// Data as the primitive type to be serialized or deserialized
  T data;

  /**
   * Create a primitive with default values. Useful for deserialization.
   */
  SerializablePrimitive() { }

  /**
   * Create serializable version of the data given
   * @param d data to use for serialization
   */
  SerializablePrimitive(const T& d) : data(d) { }

  /**
   * Length to be serialized is the memory size of the type given
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return sizeof(T);
  }

  /**
   * Serialize the data instance into the writer.
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(data);
    return writer;
  }

  /**
   * Deserialization is always done with the length of the type in the template
   * @param reader Reader to use for deserialization
   * @param length Ignored
   * @return the reader again
   * @throw std::length_error if the given length is greater than allocated
   *   data buffer
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    deserialize(reader);
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
    reader.read(data);
    return reader;
  }
};

} // namespace serialization
} // namespace eip
#endif  // ODVA_ETHERNETIP_SERIALIZATION_SERIALIZABLE_PRIMITIVE_H
