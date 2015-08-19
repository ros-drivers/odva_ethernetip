/**
Software License Agreement (proprietary)

\file      eip_encap_pkt.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_encap_pkt.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPEncapPkt::serialize(mutable_buffer buf)
{
  header_.length = buffer_size(data_);
  if (buffer_size(buf) < sizeof(header_) + header_.length)
  {
    throw std::length_error("Buffer is too small to serialize encap packet");
  }
  buffer_copy(buf, buffer(&header_, sizeof(header_)));
  buffer_copy(buf + sizeof(header_), data_);
  return sizeof(header_) + header_.length;
}

/*
size_t EIPEncapPkt::deserialize(mutable_buffer buf)
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
*/
