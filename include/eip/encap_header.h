/**
Software License Agreement (proprietary)

\file      eip_encap_header.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_ENCAP_HEADER_H
#define EIP_ENCAP_HEADER_H

#include <iostream>
#include "eip/eip_types.h"
#include "eip/serialization/serializable.h"

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Representation of an EtherNet/IP Encapsulation Packet Header
 */
class EncapHeader : public Serializable
{
public:
  EIP_UINT  command;
  EIP_UINT  length;
  EIP_UDINT session_handle;
  EIP_DWORD status;
  EIP_DWORD context[2];
  EIP_DWORD options;

  /**
   * Construct an encapsulation packet for a given command and session handle
   */
  EncapHeader(EIP_UINT cmd = 0, EIP_UDINT handle = 0)
    : command(cmd), length(0), session_handle(handle), status(0), options(0)
  {
    context[0] = 0;
    context[1] = 0;
  }

  /**
   * Always returns 24, the size of the encapsulation packer header
   * @return Total length in bytes to be serialized, always 24 bytes
   */
  virtual size_t getLength() const
  {
    return 24;
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const;

  /**
   * Deserialize data from the given reader with the length given.
   * Length must be 24 or a length_error is thrown
   * @param reader Reader to use for deserialization
   * @param length Length expected for data, must be 24
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    if (getLength() != length)
    {
      throw std::length_error("Invalid length given for encapsulation packet header");
    }
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

#endif  // EIP_ENCAP_HEADER_H
