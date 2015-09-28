/**
Software License Agreement (proprietary)

\file      eip_rr_data_cmd.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_rr_data_cmd.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

EIPRRDataCmd::EIPRRDataCmd(EIP_UDINT session_handle, EIP_USINT service_code, 
    EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id)
  : header_(EIP_CMD_SEND_RR_DATA, session_handle), service_code_(service_code)
{
  path_.addLogicalClass(class_id);
  path_.addLogicalInstance(instance_id);
  path_.addLogicalAttribute(attribute_id);
}

size_t EIPRRDataCmd::serializeMRRData(mutable_buffer buf)
{
  int num_bytes = sizeof(service_code_);
  buffer_copy(buf, buffer(&service_code_, sizeof(service_code_)));
  num_bytes += path_.serialize(buf + num_bytes);
  if (buffer_size(buf) < num_bytes + buffer_size(data_))
  {
    throw std::length_error("Buffer too short for MRR Data");
  }
  buffer_copy(buf + num_bytes, data_);
  return num_bytes + buffer_size(data_);
}

size_t EIPRRDataCmd::serialize(mutable_buffer buf)
{
  // TODO
  return 0;
}
