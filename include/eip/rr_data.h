/**
Software License Agreement (proprietary)

\file      rr_data.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_RR_DATA_H
#define EIP_RR_DATA_H

#include <string>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/serialization/serializable.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate an RR Data request or reply. Note that this is intended
 * to be used within the payload of a EncapPacket.
 */
class RRData : public Serializable
{
public:

  EIP_UDINT interface_handle;
  EIP_UINT timeout;
  // TODO: MR data

  /**
   * Construct an empty RR data holder
   */
  RRData() : interface_handle(0), timeout(0) { }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    // TODO: doesn't yet take into account size of data, since we don't have it yet
    return sizeof(interface_handle) + sizeof(timeout) + 5 * sizeof(EIP_UINT);
  }

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

#endif  // EIP_RR_DATA_H
