/**
Software License Agreement (proprietary)

\file      eip_encap_pkt.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_ENCAP_PKT_H
#define OS32C_EIP_ENCAP_PKT_H

#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using boost::asio::mutable_buffer;

/**
 * Representation of an EtherNet/IP Encapsulation Packet
 */
class EIPEncapPkt
{
public:

  /**
   * Construct an encapsulation packet
   */
  EIPEncapPkt(EIP_UINT command = 0, EIP_UDINT session_handle = 0)
    : data_(NULL, 0)
  {
    init_header(command, session_handle);
  }

  /**
   * Construct an encapsulation packet with a data payload
   * @param command Command ID for the header
   * @param session_handle Session handle to use in the header
   * @param buf Data for the payload
   */
  EIPEncapPkt(EIP_UINT command, EIP_UDINT session_handle, mutable_buffer buf)
    : data_(buf)
  {
    init_header(command, session_handle);
  }

  /**
   * Get the command value from this packet
   * @returns command value
   */
  EIP_UINT getCommand() const
  {
    return header_.command;
  }

  /**
   * Set the command value for this packet
   * @param command Command value to use
   */
  void setCommand(EIP_UINT command)
  {
    header_.command = command;
  }

  /**
   * Get the session handle from this packet
   * @returns session handle value
   */
  EIP_UDINT getSessionHandle() const
  {
    return header_.session_handle;
  }

  /**
   * Set the session handle for this packet
   * @param session_handle Session handle to use
   */
  void setSessionHandle(EIP_UDINT session_handle)
  {
    header_.session_handle = session_handle;
  }

  /**
   * Get the status from this packet
   * @returns status value
   */
  EIP_DWORD getStatus() const
  {
    return header_.status;
  }

  /**
   * Set the status for this packet
   * @param status Status value to use
   */
  void setStatus(EIP_DWORD status)
  {
    header_.status = status;
  }

  /**
   * Get the sender context value from this packet
   * @returns sender context value as a 64-bit unsigned integer (8 bytes)
   */
  uint64_t getSenderContext()
  {
    return ((uint64_t)header_.context[1] << 32) | header_.context[0];
  }

  /**
   * Set the sender context for this packet
   * @param context Context value as a 64-bit unsigned integer (8 bytes)
   */
  void setSenderContext(uint64_t context)
  {
    header_.context[0] = context & 0x00000000FFFFFFFF;
    header_.context[1] = (context >> 32) & 0x00000000FFFFFFFF;
  }

  /**
   * Get the options from this packet
   * @returns options value
   */
  EIP_DWORD getOptions() const
  {
    return header_.options;
  }

  /**
   * Set the options for this packet
   * @param options Options value to use
   */
  void setOptions(EIP_DWORD options)
  {
    header_.options = options;
  }

  /**
   * Get the length of the payload
   * @returns length value
   */
  EIP_UINT getLength() const
  {
    return buffer_size(data_);
  }

  /**
   * Get the data payload for this packet
   * @returns data payload buffer
   */
  const mutable_buffer& getData() const
  {
    return data_;
  }

  /**
   * Set the data payload for this packet
   * @param buf Buffer with data payload to use
   */
  void setData(mutable_buffer buf)
  {
    data_ = buf;
  }

  /**
   * Serialize the header to the given buffer
   * @param buf Buffer into which to serialize the header
   * @return number of bytes written to the buffer
   * @throw std::length_error if the buffer is too small for the header data
   */
  size_t serialize(mutable_buffer buf);
  /*
  {
    return boost::asio::const_buffer(&header_, sizeof(header_));
  }
  */

  /**
   * Deserialize header data to the given buffer
   * @param buf Buffer from which to read data
   * @return number of bytes read from the buffer
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  size_t deserialize(mutable_buffer buf);
  
private:
  EIP_ENCAP_HEADER header_;
  mutable_buffer data_;

  void init_header(EIP_UINT command, EIP_UDINT session_handle)
  {
    memset(&header_, 0, sizeof(header_));
    header_.command = command;
    header_.session_handle = session_handle;
  }
};

#endif  // OS32C_EIP_ENCAP_PKT_H
