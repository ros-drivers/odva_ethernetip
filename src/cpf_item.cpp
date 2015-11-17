/**
Software License Agreement (BSD)

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

#include "odva_ethernetip/cpf_item.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"
#include "odva_ethernetip/serialization/copy_serializable.h"

using boost::make_shared;

namespace eip {

using serialization::SerializableBuffer;

Writer& CPFItem::serialize(Writer& writer) const
{
  writer.write(item_type_);
  writer.write(getDataLength());
  if (item_data_) {
    item_data_->serialize(writer);
  }
  return writer;
}

Reader& CPFItem::deserialize(Reader& reader, size_t length)
{
  if (length < 4)
  {
    throw std::length_error("Length given too small to parse CPF Item type and length");
  }
  EIP_UINT item_length;
  reader.read(item_type_);
  reader.read(item_length);
  if (length < item_length + 4)
  {
    throw std::length_error("Length given too small to parse CPF Item Data");
  }
  deserializeData(reader, item_length);
  return reader;
}

Reader& CPFItem::deserialize(Reader& reader)
{
  EIP_UINT item_length;
  reader.read(item_type_);
  reader.read(item_length);
  deserializeData(reader, item_length);
  return reader;
}

void CPFItem::deserializeData(Reader& reader, EIP_UINT item_length)
{
  if (item_length > 0)
  {
    item_data_ = make_shared<SerializableBuffer>();
    item_data_->deserialize(reader, item_length);
  }
  else
  {
    item_data_ = shared_ptr<Serializable>();
  }
}

void CPFItem::getDataAs(Serializable& result)
{
  serialization::copy_serializable(result, *item_data_);
}

} // namespace eip
