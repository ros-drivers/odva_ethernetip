/**
Software License Agreement (proprietary)

\file      eip_cpf_item.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_cpf_item.h"
#include "os32c/eip_serialization_helpers.h"

std::ostream& EIPCPFItem::serialize(std::ostream& ost) const
{
  WRITE_BIN(ost, item_type_);
  EIP_UINT length = getDataLength();
  WRITE_BIN(ost, length);
  return serializeData(ost);
}

std::istream& EIPCPFItem::deserialize(std::istream& ist)
{
  EIP_UINT length;
  READ_BIN(ist, item_type_);
  READ_BIN(ist, length);
  return deserializeData(ist, length);
}

std::ostream& operator<<(std::ostream& ost, const EIPCPFItem& item)
{
  return item.serialize(ost);
}

std::istream& operator>>(std::istream& ist, EIPCPFItem& item)
{
  return item.deserialize(ist);
}