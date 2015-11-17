/**
Software License Agreement (BSD)

\file      encap_header.h
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

#ifndef ODVA_ETHERNETIP_ENCAP_HEADER_H
#define ODVA_ETHERNETIP_ENCAP_HEADER_H

#include <iostream>
#include "odva_ethernetip/eip_types.h"
#include "odva_ethernetip/serialization/reader.h"
#include "odva_ethernetip/serialization/writer.h"
#include "odva_ethernetip/serialization/serializable.h"

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

#endif  // ODVA_ETHERNETIP_ENCAP_HEADER_H
