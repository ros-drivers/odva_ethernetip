/**
Software License Agreement (proprietary)

\file      sequenced_address_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SEQUENCED_ADDRESS_ITEM_H
#define EIP_SEQUENCED_ADDRESS_ITEM_H

#include <string>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"

namespace eip {

using std::string;
using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Data structure for data portion of the session registration command and response
 */
class SequencedAddressItem : public Serializable
{
public:
  EIP_UINT connection_id;
  EIP_UINT sequence_num;

  SequencedAddressItem(EIP_UINT connection = 0, EIP_UINT sequence = 0) 
    : connection_id(connection), sequence_num(sequence) { }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return sizeof(connection_id) + sizeof(sequence_num);
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(connection_id);
    writer.write(sequence_num);
    return writer;
  }

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    deserialize(reader);
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    reader.read(connection_id);
    reader.read(sequence_num);
    return reader;
  }
};

} // namespace eip

#endif  // EIP_SEQUENCED_ADDRESS_ITEM_H
