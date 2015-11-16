/**
Software License Agreement (proprietary)

\file      sequenced_data_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SEQUENCED_DATA_ITEM_H
#define EIP_SEQUENCED_DATA_ITEM_H

#include <string>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"
#include "ovda_ethernetip/serialization/serializable.h"

namespace eip {

using std::string;
using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * For some stupid reason, sometime EIP likes to add a sequence number to the 
 * start of a data item. This template allows for easily adding that to any
 * already defined data item.
 */
template <class T>
class SequencedDataItem : public T
{
public:
  EIP_UINT sequence_num;

  SequencedDataItem(EIP_UINT sequence = 0) : sequence_num(sequence), already_deserializing(false) { }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return sizeof(sequence_num) + T::getLength();
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(sequence_num);
    return T::serialize(writer);
  }

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    if (already_deserializing) return T::deserialize(reader, length - sizeof(sequence_num));
    reader.read(sequence_num);
    already_deserializing = true;
    T::deserialize(reader, length - sizeof(sequence_num));
    already_deserializing = false;
    return reader;
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    if (already_deserializing) return T::deserialize(reader);
    reader.read(sequence_num);
    already_deserializing = true;
    T::deserialize(reader);
    already_deserializing = false;
    return reader;
  }

private:
  // TODO: this is a total hack, and probably shows that this approach isn't working
  bool already_deserializing;
};

} // namespace eip

#endif  // EIP_SEQUENCED_DATA_ITEM_H
