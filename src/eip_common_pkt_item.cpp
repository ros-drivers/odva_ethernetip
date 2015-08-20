/**
Software License Agreement (proprietary)

\file      eip_common_pkt_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_common_pkt_item.h"
#include "os32c/eip_buffer_writer.h"
#include "os32c/eip_buffer_reader.h"

using boost::asio::buffer;
using boost::asio::buffer_size;
using boost::asio::buffer_copy;
using boost::asio::buffer_cast;

size_t EIPCommonPktItem::serialize(mutable_buffer buf)
{
  EIP_UINT length = buffer_size(item_data_);
  EIPBufferWriter writer(buf);
  writer.write(item_type_);
  writer.write(length);
  writer.writeBuffer(item_data_);
  return writer.getByteCount();
}

size_t EIPCommonPktItem::deserialize(mutable_buffer buf)
{
  EIP_UINT length;
  EIPBufferReader reader(buf);
  reader.read(item_type_);
  reader.read(length);
  // intentionally casting back to mutable buffer so that item data is mutable
  item_data_ = buffer((void*)reader.readBytes(length), length);
  return reader.getByteCount();
}
