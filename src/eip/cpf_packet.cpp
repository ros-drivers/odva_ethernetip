/**
Software License Agreement (proprietary)

\file      cpf_packet.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "eip/cpf_packet.h"

using boost::make_shared;

namespace eip {

size_t CPFPacket::getLength() const
{
  // 2 bytes just for the number of items
  size_t l = 2;
  BOOST_FOREACH(CPFItem i, items_)
  {
    l += i.getLength();
  }
  return l;
}

Writer& CPFPacket::serialize(Writer& writer) const
{
  writer.write(getItemCount());
  BOOST_FOREACH(CPFItem i, items_)
  {
    i.serialize(writer);
  }
  return writer;
}

Reader& CPFPacket::deserialize(Reader& reader)
{
  items_.clear();
  EIP_UINT num_items;
  reader.read(num_items);
  for (int i = 0; i < num_items; ++i)
  {
    CPFItem item;
    item.deserialize(reader);
    items_.push_back(item);
  }
  return reader;
}

} // namespace eip
