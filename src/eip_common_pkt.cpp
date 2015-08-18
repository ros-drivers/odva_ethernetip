/**
Software License Agreement (proprietary)

\file      eip_common_pkt.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_common_pkt.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPCommonPkt::serialize(mutable_buffer buf)
{
  // TODO(kshehata): Add checking of buffer bounds
  EIP_UINT num_items = getItemCount();
  size_t total_bytes = sizeof(num_items);
  buffer_copy(buf, buffer(&num_items, sizeof(num_items)));
  for (vector<EIPCommonPktItem>::iterator it = items_.begin(); 
    it != items_.end(); ++it)
  {
    total_bytes += it->serialize(buf + total_bytes);
  }
  return total_bytes;
}

size_t EIPCommonPkt::deserialize(mutable_buffer buf)
{
  EIP_UINT num_items;
  size_t total_bytes = sizeof(num_items);
  if (buffer_size(buf) < sizeof(num_items))
  {
    throw std::length_error("Buffer too short for common packet header");
  }
  num_items = *buffer_cast<EIP_UINT*>(buf);
  items_.clear();
  for (int i = 0; i < num_items; ++i)
  {
    EIPCommonPktItem item;
    total_bytes += item.deserialize(buf + total_bytes);
    items_.push_back(item);
  }
  return total_bytes;
}
