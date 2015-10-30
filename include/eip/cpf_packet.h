/**
Software License Agreement (proprietary)

\file      cpf_packet.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_CPF_PACKET_H
#define EIP_CPF_PACKET_H

#include <vector>
#include <iostream>

#include "eip/eip_types.h"
#include "eip/cpf_item.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"

using std::vector;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Representation of an EtherNet/IP Common Packet Format packet.
 */
class CPFPacket : public Serializable
{
public:

  /**
   * Get the number of items in the packet, including both address and data items
   * @return number of items in packet
   */
  EIP_UINT getItemCount() const
  {
    return items_.size();
  }

  /**
   * Get the items in this packet
   * @return vector of the items in this packet
   */
  vector<CPFItem>& getItems()
  {
    return items_;
  }

  /**
   * Get the length in bytes of this packet after serialization
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const;

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const;

  /**
   * Length data isn't meaningful here. Instead just calls the deserialize
   * without the added information.
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
  virtual Reader& deserialize(Reader& reader);

private:
  vector< CPFItem > items_;
};

} // namespace eip

#endif  // EIP_CPF_PACKET_H
