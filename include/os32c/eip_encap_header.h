/**
Software License Agreement (proprietary)

\file      eip_encap_header.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_ENCAP_HEADER_H
#define OS32C_EIP_ENCAP_HEADER_H

#include <iostream>
#include "os32c/eip_types.h"

/**
 * Representation of an EtherNet/IP Encapsulation Packet Header
 */
struct EIPEncapHeader
{
  EIP_UINT  command;
  EIP_UINT  length;
  EIP_UDINT session_handle;
  EIP_DWORD status;
  EIP_DWORD context[2];
  EIP_DWORD options;

  /**
   * Construct an encapsulation packet for a given command and session handle
   */
  EIPEncapHeader(EIP_UINT cmd = 0, EIP_UDINT handle = 0)
    : command(cmd), length(0), session_handle(handle), status(0), options(0)
  {
    context[0] = 0;
    context[1] = 0;
  }

};

/**
 * Override to serialize Encapsulation Packet Header into a binary stream
 */
std::ostream& operator<<(std::ostream& ost, const EIPEncapHeader& header);

/**
 * Override to deserialize an Encapsulation Packet Header from a stream
 */
std::istream& operator>>(std::istream& ist, EIPEncapHeader& header);

#endif  // OS32C_EIP_ENCAP_HEADER_H
