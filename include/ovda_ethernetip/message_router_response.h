/**
Software License Agreement (BSD)

\file      message_router_response.h
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

#ifndef OVDA_ETHERNETIP_MESSAGE_ROUTER_RESPONSE_H
#define OVDA_ETHERNETIP_MESSAGE_ROUTER_RESPONSE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"

using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate a MessageRouterResponse data.
 */
class MessageRouterResponse : public Serializable
{
public:

  EIP_USINT service;
  EIP_USINT general_status;

  /**
   * Construct an empty RR data holder
   */
  MessageRouterResponse() : service(0), general_status(0) { }

  /**
   * Get the additional status data object in this message
   * @return Serializable additional status to be used for this response
   */
  shared_ptr<Serializable> getAdditionalStatus() const
  {
    return additional_status_;
  }

  /**
   * Get the response data object in this message
   * @return Serializable data to be used for this response
   */
  shared_ptr<Serializable> getResponseData() const
  {
    return response_data_;
  }

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
  virtual Writer& serialize(Writer& writer) const
  {
    throw std::logic_error("Not implemented");
  }

  /**
   * Deserialize response data from the given reader and length
   */
  virtual Reader& deserialize(Reader& reader, size_t length);

  /**
   * Not implemented. Never expect to have to receive this type of message.
   */
  virtual Reader& deserialize(Reader& reader)
  {
    throw std::logic_error("Must have size to deserialize");
  }

private:
  shared_ptr<Serializable> additional_status_;
  shared_ptr<Serializable> response_data_;

  /**
   * Helper to read data fields. Simply produces SerializableBuffers
   * of the given length, or a null pointer if length <= 0.
   * @param reader Reader to use to read buffer data
   * @param length Length in bytes to read. If less than or equal to zero, then
   *   null pointer is returned.
   * @return shared_ptr to a SerializableBuffer with the data read
   */
  shared_ptr<Serializable> readData(Reader& reader, int length);
};

} // namespace eip

#endif  // OVDA_ETHERNETIP_MESSAGE_ROUTER_RESPONSE_H
