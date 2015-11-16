/**
Software License Agreement (BSD)

\file      copy_serializable.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
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
