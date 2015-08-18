/**
Software License Agreement (proprietary)

\file      eip_common_pkt.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_COMMON_PKT_H
#define OS32C_EIP_COMMON_PKT_H

#include <boost/asio.hpp>

#include "os32c/eip_types.h"
#include "os32c/eip_common_pkt_item.h"

using std::vector;

/**
 * Representation of an EtherNet/IP Common Packet Format item.
 */
class EIPCommonPkt
{
public:

  /**
   * Get the number of items in the packet, including both address and data items
   * @return number of items in packet
   */
  EIP_UINT getItemCount()
  {
    return items_.size();
  }

  /**
   * Get the items in this packet
   * @return vector of the items in this packet
   */
  vector<EIPCommonPktItem>& getItems()
  {
    return items_;
  }

  /**
   * Serialize this packet with all of the items to the given buffer
   * @param buf buffer into which to serialize the packet
   * @return number of bytes written to buffer
   */
  size_t serialize(mutable_buffer buf);

  /**
   * Deserialize packet data from the given buffer, including all items
   * @param buf buffer from which to pull packet data
   * @return number of bytes read from buffer
   */
  size_t deserialize(mutable_buffer buf);

private:
  vector<EIPCommonPktItem> items_;
};

#endif  // OS32C_EIP_COMMON_PKT_H
