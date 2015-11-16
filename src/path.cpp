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

Path::Path(bool pad_after_length) : pad_after_length_(pad_after_length)
{
  path_buf_.reserve(6);
}

Path::Path(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id,
  bool pad_after_length) : pad_after_length_(pad_after_length)
{
  path_buf_.reserve(6);
  addLogicalClass(class_id);
  addLogicalInstance(instance_id);
  addLogicalAttribute(attribute_id);
}

Path::Path(EIP_USINT class_id, EIP_USINT instance_id) : pad_after_length_(false)
{
  path_buf_.reserve(4);
  addLogicalClass(class_id);
  addLogicalInstance(instance_id);
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

void Path::addLogicalConnectionPoint(EIP_USINT connection_id)
{
  addSegment(0x2C, connection_id);
}

size_t Path::getLength() const
{
  return sizeof(EIP_USINT) + path_buf_.size() * sizeof(EIP_USINT) + (pad_after_length_ ? 1 : 0);
}

Writer& Path::serialize(Writer& writer, bool pad_after_length) const
{
  EIP_USINT length = path_buf_.size() / 2;
  writer.write(length);
  if (pad_after_length)
  {
    EIP_BYTE reserved = 0;
    writer.write(reserved);
  }
  writer.writeBuffer(boost::asio::buffer(path_buf_));
  return writer;
}

} // namespace eip
