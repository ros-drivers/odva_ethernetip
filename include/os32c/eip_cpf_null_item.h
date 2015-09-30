/**
Software License Agreement (proprietary)

\file      eip_cpf_null_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_CPF_NULL_ITEM_H
#define OS32C_EIP_CPF_NULL_ITEM_H

#include <iostream>
#include "os32c/eip_types.h"
#include "os32c/eip_cpf_item.h"

/**
 * EIP Common Packet Null Item
 */
class EIPCPFNullItem : public EIPCPFItem
{
public:

  /**
   * Construct a null item
   */
  EIPCPFNullItem() : EIPCPFItem(EIP_ITEM_NULL) { }

  /**
   * The length of a null item is always zero
   */
  EIP_UINT getDataLength() const
  {
    return 0;
  }

protected:
  virtual std::ostream& serializeData(std::ostream& ost) const
  {
    return ost;
  }

  virtual std::istream& deserializeData(std::istream& ist, EIP_UINT length)
  {
    return ist;
  }
};

#endif  // OS32C_EIP_CPF_NULL_ITEM_H
