/**
Software License Agreement (proprietary)

\file      eip_cpf_packet.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_CPF_PACKET_H
#define OS32C_EIP_CPF_PACKET_H

#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "os32c/eip_types.h"
#include "os32c/eip_cpf_item.h"

using std::vector;
using boost::shared_ptr;

/**
 * Representation of an EtherNet/IP Common Packet Format item.
 */
class EIPCPFPacket
{
public:

  /**
   * Get the number of items in the packet, including both address and data items
   * @return number of items in packet
   */
  EIP_UINT getItemCount() const
  {
    return items_.size();
  }

  /**
   * Get the items in this packet
   * @return vector of the items in this packet
   */
  vector< shared_ptr< EIPCPFItem > >& getItems()
  {
    return items_;
  }

  /**
   * Serialize this packet as binary data
   * @param ost Output stream into which to serialize
   * @return the output stream again
   */
  std::ostream& serialize(std::ostream& ost) const;

private:
  vector< shared_ptr< EIPCPFItem > > items_;
};

std::ostream& operator<<(std::ostream& ost, const EIPCPFPacket& pkt);

#endif  // OS32C_EIP_CPF_PACKET_H
