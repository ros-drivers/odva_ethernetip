/**
Software License Agreement (proprietary)

\file      eip_path.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "eip/path.h"

#include <boost/asio.hpp>

namespace eip {

Path::Path()
{
  path_buf_.reserve(6);
}

void Path::addSegment(EIP_USINT type, EIP_USINT data)
{
  path_buf_.push_back(type);
  path_buf_.push_back(data);
}

void Path::addLogicalClass(EIP_USINT class_id)
{
  addSegment(0x20, class_id);
}

void Path::addLogicalInstance(EIP_USINT instance_id)
{
  addSegment(0x24, instance_id);
}

void Path::addLogicalAttribute(EIP_USINT attribute_id)
{
  addSegment(0x30, attribute_id);
}

size_t Path::getLength() const
{
  return sizeof(EIP_USINT) + path_buf_.size() * sizeof(EIP_USINT);
}

Writer& Path::serialize(Writer& writer) const
{
  EIP_USINT length = path_buf_.size() / 2;
  writer.write(length);
  writer.writeBuffer(boost::asio::buffer(path_buf_));
  return writer;
}

} // namespace eip
