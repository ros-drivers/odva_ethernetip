/**
Software License Agreement (proprietary)

\file      cpf_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/make_shared.hpp>

#include "eip/cpf_item.h"
#include "eip/serialization/serializable_buffer.h"

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

} // namespace eip
