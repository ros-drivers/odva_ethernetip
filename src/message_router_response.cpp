/**
Software License Agreement (BSD)

\file      message_router_response.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/make_shared.hpp>

#include "ovda_ethernetip/message_router_response.h"
#include "ovda_ethernetip/serialization/serializable_buffer.h"

using boost::make_shared;

namespace eip {

using serialization::SerializableBuffer;

size_t MessageRouterResponse::getLength() const
{
  size_t result = sizeof(service) + sizeof(EIP_USINT) + sizeof(general_status)
    + sizeof(EIP_USINT);
  if (additional_status_)
  {
    result += additional_status_->getLength();
  }
  if (response_data_)
  {
    result += response_data_->getLength();
  }
  return result;
}

Reader& MessageRouterResponse::deserialize(Reader& reader, size_t length)
{
  reader.read(service);
  // read the general status twice to skip the reserved data
  reader.read(general_status);
  reader.read(general_status);
  EIP_USINT additional_status_length;
  reader.read(additional_status_length);
  additional_status_ = readData(reader, (size_t)2*additional_status_length);
  response_data_ = readData(reader, length - reader.getByteCount());
  return reader;
}

shared_ptr<Serializable> MessageRouterResponse::readData(Reader& reader, int length)
{
  shared_ptr<SerializableBuffer> sb;
  if (length > 0)
  {
    sb = make_shared<SerializableBuffer>();
    sb->deserialize(reader, length);
  }
  return sb;
}

} // namespace eip
