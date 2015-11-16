/**
Software License Agreement (proprietary)

\file      identity_item_data.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "eip/identity_item_data.h"

namespace eip {

size_t IdentityItemData::getLength() const
{
  // TODO: verify that this is correct
  return  sizeof(encap_protocol_version)
        + sizeof(sockaddr)
        + sizeof(vendor_id)
        + sizeof(device_type)
        + sizeof(product_code)
        + sizeof(revision)
        + sizeof(status)
        + sizeof(serial_number)
        + sizeof(EIP_USINT)
        + product_name.length()
        + sizeof(state);
}

Writer& IdentityItemData::serialize(Writer& writer) const
{
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
  return writer;
}

Reader& IdentityItemData::deserialize(Reader& reader)
{
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
  product_name.resize(name_length);
  reader.readBytes(&product_name[0], name_length);
  reader.read(state);
  return reader;
}

} // namespace eip
