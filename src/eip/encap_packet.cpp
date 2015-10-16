/**
Software License Agreement (proprietary)

\file      encap_packet.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <vector>
#include <boost/make_shared.hpp>

#include "eip/encap_packet.h"
#include "eip/serialization/serializable_buffer.h"
#include "eip/serialization/buffer_reader.h"
#include "eip/serialization/buffer_writer.h"

using std::vector;
using boost::make_shared;

namespace eip {

using serialization::SerializableBuffer;
using serialization::BufferReader;
using serialization::BufferWriter;

void EncapPacket::getPayloadAs(Serializable& result)
{
  // check if this is SerializableBuffer to avoid unnecessary copy
  shared_ptr<SerializableBuffer> sb =
    boost::dynamic_pointer_cast<SerializableBuffer>(payload_);
  SerializableBuffer* result_sb = dynamic_cast<SerializableBuffer*>(&result);
  if (sb)
  {
    if (result_sb)
    {
      (*result_sb) = (*sb);
    }
    else
    {
      BufferReader reader(sb->getData());
      result.deserialize(reader, sb->getLength());
    }
    return;
  }
  else if (result_sb)
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
    size_t length = payload_->getLength();
    vector<char> buf(length);
    BufferWriter writer(buffer(buf));
    payload_->serialize(writer);
    BufferReader reader(buffer(buf));
    result.deserialize(reader, length);
  }
}

Writer& EncapPacket::serialize(Writer& writer) const
{
  // fixme: need to set length before this!
  // header_.length = (NULL == payload_) ? 0 : payload_->getLength();
  header_.serialize(writer);
  if (payload_)
  {
    payload_->serialize(writer);
  }
  return writer;
}

Reader& EncapPacket::deserialize(Reader& reader)
{
  header_.deserialize(reader);
  if (header_.length > 0)
  {
    shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>();
    sb->deserialize(reader, header_.length);
    payload_ = sb;
  }
  else
  {
    payload_ = shared_ptr<Serializable>();
  }
  return reader;
}

Reader& EncapPacket::deserialize(Reader& reader, size_t length)
{
  if (length < header_.getLength())
  {
    throw std::length_error("Deserialization size less than header size");
  }
  header_.deserialize(reader);
  if (length != header_.getLength() + header_.length)
  {
    throw std::length_error("Packet length different than given to deserialize");
  }
  if (header_.length > 0)
  {
    payload_ = make_shared<SerializableBuffer>();
    payload_->deserialize(reader, header_.length);
  }
  else
  {
    payload_ = shared_ptr<Serializable>();
  }
  return reader;
}

} // namespace eip
