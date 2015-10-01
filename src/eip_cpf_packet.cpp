/**
Software License Agreement (proprietary)

\file      eip_cpf_packet.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_cpf_packet.h"
#include <boost/foreach.hpp>
#include "os32c/eip_serialization_helpers.h"

std::ostream& EIPCPFPacket::serialize(std::ostream& ost) const
{
  EIP_UINT num_items = getItemCount();
  WRITE_BIN(ost, num_items);
  BOOST_FOREACH(shared_ptr<EIPCPFItem> i, items_)
  {
    ost << *i;
  }
  return ost;
}

std::ostream& operator<<(std::ostream& ost, const EIPCPFPacket& pkt)
{
  return pkt.serialize(ost);
}
