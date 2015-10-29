/**
Software License Agreement (proprietary)

\file      rr_data.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "eip/rr_data.h"
#include "eip/cpf_packet.h"
#include "eip/cpf_item.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

Writer& RRData::serialize(Writer& writer) const
{
  writer.write(interface_handle);
  writer.write(timeout);
  CPFPacket pkt;
  shared_ptr<CPFItem> address = make_shared<CPFItem>();
  shared_ptr<CPFItem> data = make_shared<CPFItem>();
  data->setItemType(EIP_ITEM_UNCONNECTED_MESSAGE);
  pkt.getItems().push_back(address);
  pkt.getItems().push_back(data);
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
  if (pkt.getItemCount() != 2)
  {
    throw std::logic_error("Unexpected number of items in RR Data");
  }
  if (pkt.getItems().at(0)->getItemType() != EIP_ITEM_NULL)
  {
    throw std::logic_error("Address other than null in RR Data");
  }
  if (pkt.getItems().at(0)->getDataLength() != 0)
  {
    throw std::logic_error("Data length greater than zero in null address type");
  }

  if (pkt.getItems().at(1)->getItemType() != EIP_ITEM_UNCONNECTED_MESSAGE)
  {
    throw std::logic_error("Unexpected data type in RR Data");
  }

  // TODO: parse data here
  return reader;
}

} // namespace eip
