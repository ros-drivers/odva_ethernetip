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
using boost::asio::mutable_buffer;

/**
 * Handler for CIP Identity Item data
 */
class EIPIdentityItem
{
public:

  /**
   * Get the encapsulation protocol version
   */
  EIP_UINT getEncapProtocolVersion()
  {
    return encap_protocol_version_;
  }

  /**
   * Get the socket address
   */
  sockaddr_in& getSockAddr()
  {
    return sockaddr_;
  }

  /**
   * Get the vendor ID
   */
  EIP_UINT getVendorID()
  {
    return vendor_id_;
  }

  /**
   * Get the device type
   */
  EIP_UINT getDeviceType()
  {
    return device_type_;
  }

  /**
   * Get the product code
   */
  EIP_UINT getProductCode()
  {
    return product_code_;
  }

  /**
   * Get the product revision code
   * @param segment which rev segment to return, 0 for major, 1 for minor
   */
  EIP_USINT getRevision(size_t segment)
  {
    return revision_[segment];
  }

  /**
   * Get the device status value
   */
  EIP_WORD getStatus()
  {
    return status_;
  }

  /**
   * Get the device serial number
   */
  EIP_UDINT getSerialNumber()
  {
    return serial_number_;
  }

  /**
   * Get the product name
   */
  string& getProductName()
  {
    return product_name_;
  }

  /**
   * Get the device state value
   */
  EIP_USINT getState()
  {
    return state_;
  }


  /**
   * Deserialize identity item data to the given buffer
   * @param buf Buffer from which to read data
   * @return number of bytes read from the buffer
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  size_t deserialize(mutable_buffer buf);
  
private:
  EIP_UINT encap_protocol_version_;
  sockaddr_in sockaddr_;
  EIP_UINT vendor_id_;
  EIP_UINT device_type_;
  EIP_UINT product_code_;
  EIP_USINT revision_[2];
  EIP_WORD status_;
  EIP_UDINT serial_number_;
  string product_name_;
  EIP_USINT state_;
};

#endif  // OS32C_EIP_IDENTITY_ITEM_H
