/**
Software License Agreement (BSD)

\file      copy_serializable.cpp
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

#include "ovda_ethernetip/serialization/copy_serializable.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"

namespace eip {
namespace serialization {

void copy_serializable(Serializable& dst, const Serializable& src)
{
  // check if this is SerializableBuffer to avoid unnecessary copy
  const SerializableBuffer* src_sb = dynamic_cast<const SerializableBuffer*>(&src);
  SerializableBuffer* dst_sb = dynamic_cast<SerializableBuffer*>(&dst);
  if (src_sb)
  {
    if (dst_sb)
    {
      (*dst_sb) = (*src_sb);
    }
    else
    {
      BufferReader reader(src_sb->getData());
      dst.deserialize(reader, src_sb->getLength());
    }
    return;
  }
  else if (dst_sb)
  {
    // cannot serialize to a SerializableBuffer, since it will attempt not to
    // copy the buffer but just return to it.
    // This should never be required in practice. Not implementing yet
    // TODO: fix if actually ever needed
    throw std::logic_error("Cannot get payload from Serializable to SerializableBuffer");
  }
  else
  {
    // must serialize and then deserialize again
    size_t length = src.getLength();
    std::vector<char> buf(length);
    BufferWriter writer(buffer(buf));
    src.serialize(writer);
    BufferReader reader(buffer(buf));
    dst.deserialize(reader, length);
  }
}

} // namespace serialization
} // namespace eip
