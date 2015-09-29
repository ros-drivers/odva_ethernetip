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

#define WRITE_BIN(ost, v) ost.write((char*)&v, sizeof(v))
#define READ_BIN(ist, v) ist.read((char*)&v, sizeof(v))

ostream& operator<<(ostream& ost, const EIPIdentityItem& id)
{
  WRITE_BIN(ost, id.encap_protocol_version);
  WRITE_BIN(ost, id.sockaddr);
  WRITE_BIN(ost, id.vendor_id);
  WRITE_BIN(ost, id.device_type);
  WRITE_BIN(ost, id.product_code);
  WRITE_BIN(ost, id.revision);
  WRITE_BIN(ost, id.status);
  WRITE_BIN(ost, id.serial_number);

  EIP_USINT name_length = id.product_name.size();
  WRITE_BIN(ost, name_length);
  ost << id.product_name;
  WRITE_BIN(ost, id.state);
  return ost;
}

istream& operator>>(istream& ist, EIPIdentityItem& id)
{
  READ_BIN(ist, id.encap_protocol_version);
  READ_BIN(ist, id.sockaddr);
  READ_BIN(ist, id.vendor_id);
  READ_BIN(ist, id.device_type);
  READ_BIN(ist, id.product_code);
  READ_BIN(ist, id.revision);
  READ_BIN(ist, id.status);
  READ_BIN(ist, id.serial_number);

  EIP_USINT name_length;
  READ_BIN(ist, name_length);
  id.product_name.resize(name_length);
  ist.read(&id.product_name[0], name_length);
  READ_BIN(ist, id.state);
  return ist;
}
