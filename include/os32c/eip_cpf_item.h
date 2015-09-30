/**
Software License Agreement (proprietary)

\file      eip_common_packet_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_CPF_ITEM_H
#define OS32C_EIP_CPF_ITEM_H

#include <iostream>
#include "os32c/eip_types.h"

/**
 * Base class for all common packet items
 */
class EIPCPFItem
{
public:

  /**
   * Construct a common packet item to represent the given item type.
   * @param item_type Item type code for this item (see EIP_ITEM_T)
   * @param item_data Data for this item
   */
  EIPCPFItem(EIP_UINT item_type) : item_type_(item_type) { }

  virtual ~EIPCPFItem() { }

  /**
   * Get the type code for this item
   * @return Item type code (see EIP_ITEM_T)
   */
  EIP_UINT getItemType()
  {
    return item_type_;
  }

  /**
   * Set the type code for this item
   * @param item_type Item type code (see EIP_ITEM_T)
   */
  void setItemType(EIP_UINT item_type)
  {
    item_type_ = item_type;
  }

  /**
   * Get the length of the data in this item.
   * @return length in bytes of the data for this item
   */
  virtual EIP_UINT getDataLength() const = 0;

  /**
   * Serialize this item as binary data
   * @param ost Output stream into which to serialize
   * @return the output stream again
   */
  virtual std::ostream& serialize(std::ostream& ost) const;

  /**
   * Deserialize item from the given stream
   * @param ist Input stream from which to deserialize
   * @return the input stream again
   */
  virtual std::istream& deserialize(std::istream& ist);

protected:
  /**
   * Serialize the data portion only of the item
   * @param ost Output stream into which to serialize
   * @return the output stream again
   */
  virtual std::ostream& serializeData(std::ostream& ost) const = 0;

  /**
   * Deserialize item from the given stream
   * @param ist Input stream from which to deserialize
   * @param length The length of the data as given in the common packet format
   * @return the input stream again
   */
  virtual std::istream& deserializeData(std::istream& ist, EIP_UINT length) = 0;

private:
  EIP_UINT item_type_;
};

std::ostream& operator<<(std::ostream& ost, const EIPCPFItem& item);
std::istream& operator>>(std::istream& ist, EIPCPFItem& item);

#endif  // OS32C_EIP_CPF_ITEM_H
