/**
Software License Agreement (proprietary)

\file      serializable.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SERIALIZABLE_H
#define EIP_SERIALIZABLE_H

#include "os32c/serialization/writer.h"
#include "os32c/serialization/reader.h"

namespace eip {
namespace serialization {

/**
 * Interface for all serializable data
 */
class Serializable
{
public:
  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const = 0;

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const = 0;

  /**
   * Deserialize data from the given reader with the length given
   * @param reader Reader to use for deserialization
   * @param length Length expected for data
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader, size_t length) = 0;

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader) = 0;
};

Writer& operator<<(Writer& writer, const Serializable& s);
Reader& operator>>(Reader& reader, Serializable& s);

} // namespace serialization
} // namespace eip
#endif  // EIP_SERIALIZABLE_H
