/**
Software License Agreement (proprietary)

\file      eip_common_pkt_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_COMMON_PKT_ITEM_H
#define OS32C_EIP_COMMON_PKT_ITEM_H

#include <boost/asio.hpp>

#include "os32c/eip_types.h"

using boost::asio::mutable_buffer;

/**
 * Representation of an EtherNet/IP Common Packet Format item.
 */
class EIPCommonPktItem
{
public:

  /**
   * Construct a NULL CPF Item
   */
  EIPCommonPktItem() : item_type_(EIP_ITEM_NULL), item_data_(NULL, 0) { }

  /**
   * Construct a common packet item to represent the given item type and data
   * @param item_type Item type code for this item (see EIP_TYPE_T)
   * @param item_data Data for this item
   */
  EIPCommonPktItem(EIP_UINT item_type, mutable_buffer item_data)
    : item_type_(item_type), item_data_(item_data) { }

  /**
   * Get the type code for this item
   * @return Item type code (see EIP_TYPE_T)
   */
  EIP_UINT getItemType()
  {
    return item_type_;
  }

  /**
   * Set the type code for this item
   * @param item_type Item type code (see EIP_TYPE_T)
   */
  void setItemType(EIP_UINT item_type)
  {
    item_type_ = item_type;
  }

  /**
   * Get the data for this item as a buffer
   */
  mutable_buffer getItemData()
  {
    return item_data_;
  }

  /**
   * Set the data for this item
   */
  void setItemData(mutable_buffer item_data)
  {
    item_data_ = item_data;
  }

  /**
   * Serialize this item into the given buffer
   * @param buf buffer into which to serialize this item
   * @return number of bytes written to buffer
   * @throw std::length_error if the buffer is too small to contain the item
   */
  size_t serialize(mutable_buffer buf);

  /**
   * Deserialize item data from what is given in the buffer
   * @param buf buffer from which to pull data for this item
   * @return number of bytes read from buffer
   * @throw std::length_error if run out of buffer while deserializing the item
   */
  size_t deserialize(mutable_buffer buf);

private:
  EIP_UINT item_type_;
  mutable_buffer item_data_;
};

#endif  // OS32C_EIP_COMMON_PKT_ITEM_H
