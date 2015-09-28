/**
Software License Agreement (proprietary)

\file      eip_path.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_path.h"
#include "os32c/eip_buffer_writer.h"

EIPPath::EIPPath()
{
  path_buf_.reserve(6);
}

void EIPPath::addSegment(EIP_USINT type, EIP_USINT data)
{
  path_buf_.push_back(type);
  path_buf_.push_back(data);
}

void EIPPath::addLogicalClass(EIP_USINT class_id)
{
  addSegment(0x20, class_id);
}

void EIPPath::addLogicalInstance(EIP_USINT instance_id)
{
  addSegment(0x24, instance_id);
}

void EIPPath::addLogicalAttribute(EIP_USINT attribute_id)
{
  addSegment(0x30, attribute_id);
}

size_t EIPPath::serialize(boost::asio::mutable_buffer buf)
{
  EIP_USINT length = path_buf_.size() / 2;
  EIPBufferWriter writer(buf);
  writer.write(length);
  writer.writeBuffer(boost::asio::buffer(path_buf_));
  return writer.getByteCount();
}
