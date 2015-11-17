/**
Software License Agreement (BSD)

\file      message_router_response.cpp
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

#include <boost/make_shared.hpp>

#include "ovda_ethernetip/message_router_response.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"

using boost::make_shared;

namespace eip {

using serialization::SerializableBuffer;

size_t MessageRouterResponse::getLength() const
{
  size_t result = sizeof(service) + sizeof(EIP_USINT) + sizeof(general_status)
    + sizeof(EIP_USINT);
  if (additional_status_)
  {
    result += additional_status_->getLength();
  }
  if (response_data_)
  {
    result += response_data_->getLength();
  }
  return result;
}

Reader& MessageRouterResponse::deserialize(Reader& reader, size_t length)
{
  reader.read(service);
  // read the general status twice to skip the reserved data
  reader.read(general_status);
  reader.read(general_status);
  EIP_USINT additional_status_length;
  reader.read(additional_status_length);
  additional_status_ = readData(reader, (size_t)2*additional_status_length);
  response_data_ = readData(reader, length - reader.getByteCount());
  return reader;
}

shared_ptr<Serializable> MessageRouterResponse::readData(Reader& reader, int length)
{
  shared_ptr<SerializableBuffer> sb;
  if (length > 0)
  {
    sb = make_shared<SerializableBuffer>();
    sb->deserialize(reader, length);
  }
  return sb;
}

} // namespace eip
