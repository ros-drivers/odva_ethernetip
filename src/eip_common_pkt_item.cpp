/**
Software License Agreement (proprietary)

\file      eip_common_pkt_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_common_pkt_item.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPCommonPktItem::serialize(mutable_buffer buf)
{
  // TODO(kshehata): Add checking of buffer bounds to this!
  EIP_UINT length = buffer_size(item_data_);
  if (buffer_size(buf) < sizeof(item_type_) + sizeof(length) + length)
  {
    throw std::length_error("Buffer too short for item data");
  }
  buffer_copy(buf, buffer(&item_type_, sizeof(item_type_)));
  buffer_copy(buf + sizeof(item_type_), buffer(&length, sizeof(length)));
  buffer_copy(buf + sizeof(item_type_) + sizeof(length), item_data_);
  return sizeof(item_type_) + sizeof(length) + length;
}

size_t EIPCommonPktItem::deserialize(mutable_buffer buf)
{
  EIP_UINT length;
  if (buffer_size(buf) < sizeof(item_type_) + sizeof(length))
  {
    throw std::length_error("Buffer too short for item header");
  }

  item_type_ = *buffer_cast<EIP_UINT*>(buf);
  buf = buf + sizeof(item_type_);
  length = *buffer_cast<EIP_UINT*>(buf);
  buf = buf + sizeof(length);

  if (buffer_size(buf) < length)
  {
    throw std::length_error("Buffer too short for item data");
  }

  item_data_ = buffer(buf, length);
  return sizeof(item_type_) + sizeof(length) + length;
}
