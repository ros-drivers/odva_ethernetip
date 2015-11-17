/**
Software License Agreement (BSD)

\file      sequenced_data_item.h
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

#ifndef OVDA_ETHERNETIP_SEQUENCED_DATA_ITEM_H
#define OVDA_ETHERNETIP_SEQUENCED_DATA_ITEM_H

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
 * For some stupid reason, sometime EIP likes to add a sequence number to the
 * start of a data item. This template allows for easily adding that to any
 * already defined data item.
 */
template <class T>
class SequencedDataItem : public T
{
public:
  EIP_UINT sequence_num;

  SequencedDataItem(EIP_UINT sequence = 0) : sequence_num(sequence), already_deserializing(false) { }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return sizeof(sequence_num) + T::getLength();
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(sequence_num);
    return T::serialize(writer);
  }

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    if (already_deserializing) return T::deserialize(reader, length - sizeof(sequence_num));
    reader.read(sequence_num);
    already_deserializing = true;
    T::deserialize(reader, length - sizeof(sequence_num));
    already_deserializing = false;
    return reader;
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    if (already_deserializing) return T::deserialize(reader);
    reader.read(sequence_num);
    already_deserializing = true;
    T::deserialize(reader);
    already_deserializing = false;
    return reader;
  }

private:
  // TODO: this is a total hack, and probably shows that this approach isn't working
  bool already_deserializing;
};

} // namespace eip

#endif  // OVDA_ETHERNETIP_SEQUENCED_DATA_ITEM_H
