/**
Software License Agreement (proprietary)

\file      eip_rr_data_cmd.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_RR_DATA_CMD_H
#define OS32C_EIP_RR_DATA_CMD_H

#include <string>
#include <boost/asio.hpp>

#include "os32c/eip_types.h"
#include "os32c/eip_encap_pkt.h"
#include "os32c/eip_path.h"

/**
 * Class to handle creating and managing Ethernet/IP Sessions
 */
class EIPRRDataCmd
{
public:

  /**
   * Construct an RR Data Command with the given info
   * @param session_handle The handle for the current session
   * @param service_code The service code to be sent in the MRR
   * @param class_id The class ID of the destination
   * @param instance_id The instance ID of the destination
   * @param attribute_id The attribute ID of the destination
   */
  EIPRRDataCmd(EIP_UDINT session_handle, EIP_USINT service_code, 
    EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id);

  /**
   * Serialize the command into the given buffer
   * @param buf buffer into which to serialize the command
   * @return number of bytes written to buffer
   * @throw std::length_error if the buffer is too small to contain the command
   */
  size_t serialize(boost::asio::mutable_buffer buf);

  size_t serializeMRRData(mutable_buffer buf);

private:
  EIPEncapPkt header_;
  EIP_USINT service_code_;
  EIPPath path_;
  mutable_buffer data_;
};

#endif  // OS32C_EIP_RR_DATA_CMD_H
