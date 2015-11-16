/**
Software License Agreement (BSD)

\file      forward_open_request.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_FORWARD_OPEN_REQUEST_H
#define EIP_FORWARD_OPEN_REQUEST_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"
#include "ovda_ethernetip/path.h"

using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

typedef enum
{
  CONN_PRIORITY_LOW       = 0,
  CONN_PRIORITY_HIGH      = 1,
  CONN_PRIORITY_SCHEDULED = 2,
  CONN_PRIORITY_URGENT    = 3,
} CONN_PRIORITY_T;

typedef enum
{
  CONN_TYPE_NULL      = 0,
  CONN_TYPE_MULTICAST = 1,
  CONN_TYPE_P2P       = 2,
  CONN_TYPE_RESERVED  = 3,
} CONN_TYPE_T;

/**
 * Class to encapsulate a ForwardOpenRequest data. Note that this is currently
 * only LARGE forward open, but could be easily changed to support both.
 */
class ForwardOpenRequest : public Serializable
{
public:

  EIP_BYTE timeout_tick_size;
  EIP_USINT timeout_ticks;
  EIP_UDINT o_to_t_connection_id;
  EIP_UDINT t_to_o_connection_id;
  EIP_UINT connection_sn;
  EIP_UINT originator_vendor_id;
  EIP_UDINT originator_sn;
  EIP_USINT timeout_multiplyer;
  EIP_UDINT o_to_t_rpi;
  EIP_DWORD o_to_t_conn_params;
  EIP_UDINT t_to_o_rpi;
  EIP_DWORD t_to_o_conn_params;
  EIP_BYTE conn_type;

  /**
   * Helper to calculate connection parameters
   * @param size Maximum size of the messages in the connection in byte
   * @param variable if set to true, variable message sizes
   * @param priority Priority value for the connection
   * @param type Connection type / class info
   * @param shared If set to true, then a shared connection
   */
  static EIP_DWORD calcConnectionParams(EIP_UINT size, bool variable, EIP_BYTE priority, 
    EIP_BYTE type, bool shared)
  {
    return (size & 0x7FFF) | (variable ? 0x2000000 : 0) | (priority & 0x03) << 26
      | (type & 0x03) << 29 | (shared ? 0x80000000 : 0);
  }

  /**
   * Shortcut to set the origin to target parameters.
   */
  EIP_DWORD setOriginToTargetParams(EIP_UINT size, bool variable, EIP_BYTE priority, 
    EIP_BYTE type, bool shared)
  {
    o_to_t_conn_params = calcConnectionParams(size, variable, priority, type, shared);
    return 0;
  }

  /**
   * Shortcut to set the target to origin params.
   */
  EIP_DWORD setTargetToOriginParams(EIP_UINT size, bool variable, EIP_BYTE priority, 
    EIP_BYTE type, bool shared)
  {
    t_to_o_conn_params = calcConnectionParams(size, variable, priority, type, shared);
    return 0;
  }

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
          + sizeof(o_to_t_connection_id)
          + sizeof(t_to_o_connection_id)
          + sizeof(connection_sn)
          + sizeof(originator_vendor_id)
          + sizeof(originator_sn)
          + sizeof(timeout_multiplyer)
          + sizeof(o_to_t_rpi)
          + sizeof(o_to_t_conn_params)
          + sizeof(t_to_o_rpi)
          + sizeof(t_to_o_conn_params)
          + sizeof(conn_type)
          + 3 // reserved bytes
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
    EIP_BYTE reserved = 0;
    writer.write(timeout_tick_size);
    writer.write(timeout_ticks);
    writer.write(o_to_t_connection_id);
    writer.write(t_to_o_connection_id);
    writer.write(connection_sn);
    writer.write(originator_vendor_id);
    writer.write(originator_sn);
    writer.write(timeout_multiplyer);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(o_to_t_rpi);
    writer.write(o_to_t_conn_params);
    writer.write(t_to_o_rpi);
    writer.write(t_to_o_conn_params);
    writer.write(conn_type);
    path_.serialize(writer);
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
  Path path_;
};

} // namespace eip

#endif  // EIP_FORWARD_OPEN_REQUEST_H
