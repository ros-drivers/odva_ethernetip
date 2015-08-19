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

size_t EIPEncapPkt::deserialize(mutable_buffer buf)
{
  if (buffer_size(buf) < sizeof(header_))
  {
    throw std::length_error("Buffer is too small to contain encap packet header");
  }
  buffer_copy(buffer(&header_, sizeof(header_)), buf);
  data_ = buffer(buf + sizeof(header_), header_.length);
  if (buffer_size(data_) != header_.length)
  {
    throw std::length_error("Ran out of buffer before end of payload data");
  }
  return sizeof(header_) + header_.length;
}
