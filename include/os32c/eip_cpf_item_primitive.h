/**
Software License Agreement (proprietary)

\file      eip_cpf_item_primitive.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_CPF_ITEM_PRIMITIVE_H
#define OS32C_EIP_CPF_ITEM_PRIMITIVE_H

#include <iostream>
#include "os32c/eip_types.h"
#include "os32c/eip_cpf_item.h"

/**
 * EIP Common Packet Item that is just a simple type
 */
template <typename T>
class EIPCPFItemPrimitive : public EIPCPFItem
{
public:

  /**
   * Construct a common packet item to represent the given item type and data
   * @param item_type Item type code for this item (see EIP_TYPE_T)
   * @param item_data Data for this item
   */
  EIPCPFItemPrimitive(EIP_UINT item_type, const T& item_data)
    : EIPCPFItem(item_type), item_data_(item_data) { }

  /**
   * Get the data for this item
   */
  T getItemData()
  {
    return item_data_;
  }

  /**
   * Set the data for this item
   */
  void setItemData(const T& item_data)
  {
    item_data_ = item_data;
  }

  /**
   * Get the length of the primitive item
   */
  EIP_UINT getDataLength() const
  {
    return sizeof(item_data_);
  }

protected:
  /**
   * Serialize the data portion only of the item
   * @param ost Output stream into which to serialize
   * @return the output stream again
   */
  virtual std::ostream& serializeData(std::ostream& ost) const
  {
    ost.write((char*)&item_data_, sizeof(item_data_));
  }

  /**
   * Deserialize item from the given stream
   * @param ist Input stream from which to deserialize
   * @param length The length of the data as given in the common packet format
   * @return the input stream again
   */
  virtual std::istream& deserializeData(std::istream& ist, EIP_UINT length)
  {
    // TODO: Check that length is the same as the size of item data!
    ist.read((char*)&item_data_, sizeof(item_data_));
  }

private:
  T item_data_;
};

#endif  // OS32C_EIP_CPF_ITEM_PRIMITIVE_H
