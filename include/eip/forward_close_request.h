/**
Software License Agreement (proprietary)

\file      forward_close_request.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_FORWARD_CLOSE_REQUEST_H
#define EIP_FORWARD_CLOSE_REQUEST_H

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
 * Class to encapsulate a Forward Close request.
 */
class ForwardCloseRequest : public Serializable
{
public:

  EIP_BYTE timeout_tick_size;
  EIP_USINT timeout_ticks;
  EIP_UINT connection_sn;
  EIP_UINT originator_vendor_id;
  EIP_UDINT originator_sn;

  /**
   * Get the path in the given message router request
   * @return reference to the current Path
   */
  Path& getPath()
  {
    return path_;
  }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return sizeof(timeout_tick_size)
          + sizeof(timeout_ticks)
          + sizeof(connection_sn)
          + sizeof(originator_vendor_id)
          + sizeof(originator_sn)
          + 1 // reserved byte
          + path_.getLength();
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    writer.write(timeout_tick_size);
    writer.write(timeout_ticks);
    writer.write(connection_sn);
    writer.write(originator_vendor_id);
    writer.write(originator_sn);
    // add reserved byte should be after the path size.
    // seriously. Who thought this out? Dumbest standard ever.
    path_.serialize(writer, true);
  }

  /**
   * Not implemented. Never expect to have to receive this type of message.
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    std::logic_error("Not implemented");
  }

  /**
   * Not implemented. Never expect to have to receive this type of message.
   */
  virtual Reader& deserialize(Reader& reader)
  {
    std::logic_error("Not implemented");
  }

private:
  Path path_;
};

} // namespace eip

#endif  // EIP_FORWARD_CLOSE_REQUEST_H
