/**
Software License Agreement (proprietary)

\file      eip_identity_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_identity_item.h"
#include "os32c/eip_buffer_reader.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPIdentityItem::deserialize(mutable_buffer buf)
{
  EIPBufferReader reader(buf);
  reader.read(encap_protocol_version_);
  reader.read(sockaddr_);
  reader.read(vendor_id_);
  reader.read(device_type_);
  reader.read(product_code_);
  reader.read(revision_);
  reader.read(status_);
  reader.read(serial_number_);

  EIP_USINT name_length;
  reader.read(name_length);
  product_name_ = string(reader.readBytes(name_length), name_length);
  reader.read(state_);
  return reader.getByteCount();
}
