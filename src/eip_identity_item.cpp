/**
Software License Agreement (proprietary)

\file      eip_identity_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_identity_item.h"
#include "os32c/eip_buffer_writer.h"
#include "os32c/eip_buffer_reader.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPIdentityItem::serialize(boost::asio::mutable_buffer buf)
{
  EIPBufferWriter writer(buf);
  writer.write(encap_protocol_version);
  writer.write(sockaddr);
  writer.write(vendor_id);
  writer.write(device_type);
  writer.write(product_code);
  writer.write(revision);
  writer.write(status);
  writer.write(serial_number);

  EIP_USINT name_length = product_name.size();
  writer.write(name_length);
  writer.writeBytes(product_name.c_str(), name_length);
  writer.write(state);
  return writer.getByteCount();
}

size_t EIPIdentityItem::deserialize(boost::asio::const_buffer buf)
{
  EIPBufferReader reader(buf);
  reader.read(encap_protocol_version);
  reader.read(sockaddr);
  reader.read(vendor_id);
  reader.read(device_type);
  reader.read(product_code);
  reader.read(revision);
  reader.read(status);
  reader.read(serial_number);

  EIP_USINT name_length;
  reader.read(name_length);
  product_name = string(reader.readBytes(name_length), name_length);
  reader.read(state);
  return reader.getByteCount();
}
