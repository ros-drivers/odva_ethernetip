/**
Software License Agreement (BSD)

\file      rr_data.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <console_bridge/console.h>

#include "odva_ethernetip/rr_data.h"
#include "odva_ethernetip/cpf_packet.h"
#include "odva_ethernetip/cpf_item.h"

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
    CONSOLE_BRIDGE_logWarn("More than 2 items in RR data response");
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
