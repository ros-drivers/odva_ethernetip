/**
Software License Agreement (proprietary)

\file      message_router_request.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_MESSAGE_ROUTER_REQUEST_H
#define EIP_MESSAGE_ROUTER_REQUEST_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "eip/eip_types.h"
#include "eip/serialization/serializable.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/path.h"

using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate a MessageRouterRequest data.
 */
class MessageRouterRequest : public Serializable
{
public:

  EIP_USINT service;

  /**
   * Construct an empty RR data holder
   */
  MessageRouterRequest(EIP_USINT service_code = 0) : service(service_code) { }

  /**
   * Get the path in the given message router request
   * @return reference to the current Path
   */
  Path& getPath()
  {
    return path_;
  }

  /**
   * Set the data to be used for this message router request
   * @param data Serializable that will be used as the data object
   */
  void setData(shared_ptr<Serializable> data)
  {
    data_ = data;
  }

  /**
   * Get the data object in this message request
   * @return Serializable data to be used for this request
   */
  shared_ptr<Serializable> getData() const
  {
    return data_;
  }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    size_t result = sizeof(service) + path_.getLength();
    if (data_)
    {
      result += data_->getLength();
    }
    return result;
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(service);
    path_.serialize(writer);
    if (data_) {
      data_->serialize(writer);
    }
    return writer;
  }

  /**
   * Not implemented. Never expect to have to receive this type of message.
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    throw std::logic_error("Not implemented");
  }

  /**
   * Not implemented. Never expect to have to receive this type of message.
   */
  virtual Reader& deserialize(Reader& reader)
  {
    throw std::logic_error("Not implemented");
  }

private:
  shared_ptr<Serializable> data_;
  Path path_;
};

} // namespace eip

#endif  // EIP_MESSAGE_ROUTER_REQUEST_H
