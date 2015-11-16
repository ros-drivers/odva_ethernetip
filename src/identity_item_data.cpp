/**
Software License Agreement (BSD)

\file      identity_item_data.cpp
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

#include "ovda_ethernetip/identity_item_data.h"

namespace eip {

size_t IdentityItemData::getLength() const
{
  // TODO: verify that this is correct
  return  sizeof(encap_protocol_version)
        + sizeof(sockaddr)
        + sizeof(vendor_id)
        + sizeof(device_type)
        + sizeof(product_code)
        + sizeof(revision)
        + sizeof(status)
        + sizeof(serial_number)
        + sizeof(EIP_USINT)
        + product_name.length()
        + sizeof(state);
}

Writer& IdentityItemData::serialize(Writer& writer) const
{
  writer.write(encap_protocol_version);
  writer.write(sockaddr);
  writer.write(vendor_id);
  writer.write(device_type);
  writer.write(product_code);
  writer.write(revision);
  writer.write(status);
  writer.write(serial_number);

  EIP_USINT name_length = product_name.size();
  writer.write(name_length);
  writer.writeBytes(product_name.c_str(), name_length);
  writer.write(state);
  return writer;
}

Reader& IdentityItemData::deserialize(Reader& reader)
{
  reader.read(encap_protocol_version);
  reader.read(sockaddr);
  reader.read(vendor_id);
  reader.read(device_type);
  reader.read(product_code);
  reader.read(revision);
  reader.read(status);
  reader.read(serial_number);

  EIP_USINT name_length;
  reader.read(name_length);
  product_name.resize(name_length);
  reader.readBytes(&product_name[0], name_length);
  reader.read(state);
  return reader;
}

} // namespace eip
