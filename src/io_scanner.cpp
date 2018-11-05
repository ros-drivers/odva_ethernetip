/**
Software License Agreement (BSD)

\file      io_scanner.cpp
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

#include <iostream>
#include <boost/bind.hpp>
#include <console_bridge/console.h>

#include "odva_ethernetip/io_scanner.h"
#include "odva_ethernetip/eip_types.h"
#include "odva_ethernetip/serialization/buffer_reader.h"
#include "odva_ethernetip/serialization/buffer_writer.h"
#include "odva_ethernetip/encap_packet.h"
#include "odva_ethernetip/cpf_packet.h"
#include "odva_ethernetip/cpf_item.h"
#include "odva_ethernetip/identity_item_data.h"

using namespace boost::asio;
using boost::asio::ip::udp;
using std::endl;

namespace eip {

using serialization::BufferReader;
using serialization::BufferWriter;

IOScanner::IOScanner(io_service& io_service, string hostname)
    : socket_(io_service), hostname_(hostname)
{
  CONSOLE_BRIDGE_logInform("Opening UDP socket... ");
  socket_.open(udp::v4());
  socket_.async_receive_from(buffer(recv_buf_), device_endpoint_,
    boost::bind(&IOScanner::handleListIdentityResponse, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  CONSOLE_BRIDGE_logInform("done.");
}

void IOScanner::sendListIdentityRequest()
{
  CONSOLE_BRIDGE_logInform("Sending List Identity Request... ");
  udp::resolver r(socket_.get_io_service());
  udp::resolver::query q(udp::v4(), hostname_, "44818");
  udp::endpoint receiver_endpoint = *r.resolve(q);

  EncapPacket pkt(EIP_CMD_LIST_IDENTITY);
  char d[128];
  BufferWriter w(buffer(d));
  pkt.serialize(w);
  socket_.send_to(buffer(d, w.getByteCount()), receiver_endpoint);
  CONSOLE_BRIDGE_logInform("done.");
}

void IOScanner::handleListIdentityResponse(const boost::system::error_code& ec,
  std::size_t num_bytes)
{
  if (ec)
  {
    CONSOLE_BRIDGE_logError("Error receiving list identity response message");
    return;
  }

  try
  {
    BufferReader r(buffer(recv_buf_, num_bytes));
    EncapPacket pkt;
    pkt.deserialize(r);
    if (r.getByteCount() != num_bytes)
    {
      CONSOLE_BRIDGE_logWarn("Packet received with %zu bytes, but only %zu bytes used", num_bytes, r.getByteCount());
    }

    if (pkt.getHeader().command != EIP_CMD_LIST_IDENTITY)
    {
      CONSOLE_BRIDGE_logError("Reply received with wrong command. Expected %u, received %u", EIP_CMD_LIST_IDENTITY,
                              pkt.getHeader().command);
      return;
    }
    if (pkt.getHeader().session_handle != 0)
    {
      CONSOLE_BRIDGE_logWarn("Non-zero session handle received: %zu", pkt.getHeader().session_handle);
    }
    if (pkt.getHeader().status != 0)
    {
      CONSOLE_BRIDGE_logWarn("Non-zero status received: %zu", pkt.getHeader().status);
    }
    if (pkt.getHeader().context[0] != 0 || pkt.getHeader().context[1] != 0)
    {
      CONSOLE_BRIDGE_logWarn("Non-zero sender context received: %zu, %zu", pkt.getHeader().context[0],
                             pkt.getHeader().context[1]);
    }
    if (pkt.getHeader().options != 0)
    {
      CONSOLE_BRIDGE_logWarn("Non-zero options received: %zu", pkt.getHeader().options);
    }

    CPFPacket payload;
    pkt.getPayloadAs(payload);

    if (payload.getItemCount() < 1)
    {
      CONSOLE_BRIDGE_logError("No items in list identity payload!");
      return;
    }
    if (payload.getItemCount() > 1)
    {
      CONSOLE_BRIDGE_logWarn("More than one item in list identity payload %u", payload.getItemCount());
    }

    if (payload.getItems().at(0).getItemType() != EIP_ITEM_LIST_IDENTITY_RESPONSE)
    {
      CONSOLE_BRIDGE_logError("Error: Payload response received with the wrong item type. Expected: %zu, received %zu",
               EIP_ITEM_LIST_IDENTITY_RESPONSE, payload.getItems().at(0).getItemType());
      return;
    }

    IdentityItemData id;
    payload.getItems().at(0).getDataAs(id);

    CONSOLE_BRIDGE_logInform("=== Received ID Message ===");
    CONSOLE_BRIDGE_logInform("Encapsulation Protocol Version: %d", (int)id.encap_protocol_version);
    CONSOLE_BRIDGE_logInform("Address: %d : %d", inet_ntoa(id.sockaddr.sin_addr), ntohs(id.sockaddr.sin_port));
    CONSOLE_BRIDGE_logInform("Vendor ID: %d", (int)id.vendor_id);
    CONSOLE_BRIDGE_logInform("Device Type: %d", (int)id.device_type);
    CONSOLE_BRIDGE_logInform("Product Code: %d", (int)id.product_code);
    CONSOLE_BRIDGE_logInform("Revision: %d.%d", (int)id.revision[0], (int)id.revision[1]);
    CONSOLE_BRIDGE_logInform("Status: %d", (int)id.status);
    CONSOLE_BRIDGE_logInform("Serial Number: %d", (int)id.serial_number);
    CONSOLE_BRIDGE_logInform("Product Name: %s", id.product_name.c_str());
    CONSOLE_BRIDGE_logInform("State: %d", (int)id.state);
  }
  catch (std::length_error e)
  {
    printf("ERROR: Packet too short for identity response\n");
  }
}

void IOScanner::run()
{
  sendListIdentityRequest();
  CONSOLE_BRIDGE_logInform("Waiting for responses.");
  socket_.get_io_service().run();
}

} // namespace eip
