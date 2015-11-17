/**
Software License Agreement (BSD)

\file      encap_packet.h
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

#ifndef ODVA_ETHERNETIP_ENCAP_PACKET_H
#define ODVA_ETHERNETIP_ENCAP_PACKET_H

#include <boost/shared_ptr.hpp>

#include "odva_ethernetip/eip_types.h"

#include "odva_ethernetip/encap_header.h"
#include "odva_ethernetip/serialization/reader.h"
#include "odva_ethernetip/serialization/writer.h"
#include "odva_ethernetip/serialization/serializable.h"


using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Representation of an EtherNet/IP Encapsulation Packet
 */
class EncapPacket : public Serializable
{
public:

  /**
   * Construct an encapsulation packet
   */
  EncapPacket(EIP_UINT command = 0, EIP_UDINT session_handle = 0)
    : header_(command, session_handle) { }

  /**
   * Construct an encapsulation packet with a data payload
   * @param command Command ID for the header
   * @param session_handle Session handle to use in the header
   * @param buf Data for the payload
   */
  EncapPacket(EIP_UINT command, EIP_UDINT session_handle,
    shared_ptr<Serializable> payload)
    : header_(command, session_handle), payload_(payload)
    {
      updateLength();
    }


  /**
   * Get the header portion of this packet
   * @return Header data
   */
  EncapHeader& getHeader()
  {
    return header_;
  }

  /**
   * Get the data payload from this packet
   * @return the data payload of the packet
   */
  shared_ptr<Serializable> getPayload() const
  {
    return payload_;
  }

  /**
   * Set the payload for this packet
   * @param payload Payload to use
   */
  void setPayload(shared_ptr<Serializable> payload)
  {
    payload_ = payload;
    updateLength();
  }

  /**
   * Get the payload as a particular serializable instance
   * @param result Serializable class to get data into
   */
  void getPayloadAs(Serializable& result);

  /**
   * Get the length of the entire packet, not just the payload
   * @return total length of the header and the payload combined
   */
  size_t getLength() const
  {
    size_t l = header_.getLength();
    if (NULL != payload_)
    {
      l += payload_->getLength();
    }
    return l;
  }

  /**
   * Update the length field of the header. Note that this must be called if
   * the payload size is modified after setting the payload reference.
   */
  void updateLength()
  {
    header_.length = payload_ ? payload_->getLength() : 0;
  }

  /**
   * Serialize the packet to the given writer
   * @param writer The Writer into which to serialize
   * @return the writer again
   */
  Writer& serialize(Writer& writer) const;

  /**
   * Deserialize packet from the given reader with the max length given
   * @param reader Reader to use for deserialization
   * @param length Length expected for the packet
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  Reader& deserialize(Reader& reader, size_t length);

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  Reader& deserialize(Reader& reader);

private:
  EncapHeader header_;
  shared_ptr<Serializable> payload_;
};

} // namespace eip

#endif  // ODVA_ETHERNETIP_ENCAP_PACKET_H
