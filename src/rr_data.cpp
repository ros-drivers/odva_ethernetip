/**
Software License Agreement (BSD)

\file      rr_data.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "ovda_ethernetip/rr_data.h"
#include "ovda_ethernetip/cpf_packet.h"
#include "ovda_ethernetip/cpf_item.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

Writer& RRData::serialize(Writer& writer) const
{
  writer.write(interface_handle);
  writer.write(timeout);
  CPFPacket pkt;
  pkt.getItems().push_back(CPFItem());
  pkt.getItems().push_back(CPFItem(EIP_ITEM_UNCONNECTED_MESSAGE, getData()));
  pkt.serialize(writer);
  return writer;
}

Reader& RRData::deserialize(Reader& reader)
{
  reader.read(interface_handle);
  reader.read(timeout);
  CPFPacket pkt;
  pkt.deserialize(reader);

  // check that CPF items are correct
  if (pkt.getItemCount() < 2)
  {
    throw std::logic_error("Not enough items in RR Data");
  }
  if (pkt.getItemCount() > 2)
  {
    std::cout << "Warning: more than 2 items in RR data response" << std::endl;
  }
  if (pkt.getItems().at(0).getItemType() != EIP_ITEM_NULL)
  {
    throw std::logic_error("Address other than null in RR Data");
  }
  if (pkt.getItems().at(0).getDataLength() != 0)
  {
    throw std::logic_error("Data length greater than zero in null address type");
  }

  if (pkt.getItems().at(1).getItemType() != EIP_ITEM_UNCONNECTED_MESSAGE)
  {
    throw std::logic_error("Unexpected data type in RR Data");
  }

  setData(pkt.getItems().at(1));
  return reader;
}

} // namespace eip
