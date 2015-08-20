/**
Software License Agreement (proprietary)

\file      eip_identity_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_IDENTITY_ITEM_H
#define OS32C_EIP_IDENTITY_ITEM_H

#include <string>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using std::string;

/**
 * Handler for CIP Identity Item data.
 * 
 * Note that I'm intentionally making this like a struct. It's basically a
 * a struct with serialization and deserialization capabilities.
 * Not sure if I prefer this layout, or hiding data behind accessors.
 */
class EIPIdentityItem
{
public:
  EIP_UINT encap_protocol_version;
  sockaddr_in sockaddr;
  EIP_UINT vendor_id;
  EIP_UINT device_type;
  EIP_UINT product_code;
  EIP_USINT revision[2];
  EIP_WORD status;
  EIP_UDINT serial_number;
  string product_name;
  EIP_USINT state;

  /**
   * Serialize identity item data to the given buffer
   * @param buf Buffer to which to write data
   * @return number of bytes written to the buffer
   * @throw std::length_error if the buffer is overrun while serializing
   */
  size_t serialize(boost::asio::mutable_buffer buf);

  /**
   * Deserialize identity item data from the given buffer
   * @param buf Buffer from which to read data
   * @return number of bytes read from the buffer
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  size_t deserialize(boost::asio::const_buffer buf);  
};

#endif  // OS32C_EIP_IDENTITY_ITEM_H
