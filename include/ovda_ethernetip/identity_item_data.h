/**
Software License Agreement (proprietary)

\file      identity_item_data.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_IDENTITY_ITEM_DATA_H
#define EIP_IDENTITY_ITEM_DATA_H

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
 * Data structure and operators for CIP Identity Item data.
 */
class IdentityItemData : public Serializable
{
public:
  EIP_UINT encap_protocol_version;
  sockaddr_in sockaddr;
  EIP_UINT vendor_id;
  EIP_UINT device_type;
  EIP_UINT product_code;
  EIP_USINT revision[2];
  EIP_WORD status;
  EIP_UDINT serial_number;
  string product_name;
  EIP_USINT state;

  /**
   * Get the length of serialized data that would be produced if serialized
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
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    deserialize(reader);
    return reader;
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader);
};

} // namespace eip

#endif  // EIP_IDENTITY_ITEM_DATA_H
