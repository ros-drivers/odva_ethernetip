/**
Software License Agreement (proprietary)

\file      io_scanner.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <iostream>
#include <boost/bind.hpp>

#include "ovda_ethernetip/io_scanner.h"
#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/buffer_reader.h"
#include "ovda_ethernetip/serialization/buffer_writer.h"
#include "ovda_ethernetip/encap_packet.h"
#include "ovda_ethernetip/cpf_packet.h"
#include "ovda_ethernetip/cpf_item.h"
#include "ovda_ethernetip/identity_item_data.h"

using namespace boost::asio;
using boost::asio::ip::udp;
using std::cerr;
using std::cout;
using std::endl;

namespace eip {

using serialization::BufferReader;
using serialization::BufferWriter;

IOScanner::IOScanner(io_service& io_service, string hostname)
    : socket_(io_service), hostname_(hostname)
{
  cout << "Opening UDP socket... ";
  socket_.open(udp::v4());
  socket_.async_receive_from(buffer(recv_buf_), device_endpoint_,
    boost::bind(&IOScanner::handleListIdentityResponse, this, 
      boost::asio::placeholders::error, 
      boost::asio::placeholders::bytes_transferred));
  cout << "done." << endl;
}

void IOScanner::sendListIdentityRequest()
{
  cout << "Sending List Identity Request... ";
  udp::resolver r(socket_.get_io_service());
  udp::resolver::query q(udp::v4(), hostname_, "44818");
  udp::endpoint receiver_endpoint = *r.resolve(q);

  EncapPacket pkt(EIP_CMD_LIST_IDENTITY);
  char d[128];
  BufferWriter w(buffer(d));
  pkt.serialize(w);
  socket_.send_to(buffer(d, w.getByteCount()), receiver_endpoint);
  cout << "done." << endl;
}

void IOScanner::handleListIdentityResponse(const boost::system::error_code& ec,
  std::size_t num_bytes)
{
  if (ec)
  {
    cerr << "Error receiving list identity response message" << endl;
    return;
  }

  try
  {
    BufferReader r(buffer(recv_buf_, num_bytes));
    EncapPacket pkt;
    pkt.deserialize(r);
    if (r.getByteCount() != num_bytes)
    {
      cerr << "Warning: packet received with " << num_bytes << 
        " bytes, but only " << r.getByteCount() << " bytes used" << endl;
    }

    if (pkt.getHeader().command != EIP_CMD_LIST_IDENTITY)
    {
      cerr << "Reply received with wrong command. Expected " 
        << EIP_CMD_LIST_IDENTITY << ", received " << pkt.getHeader().command << endl;
      return;
    }
    if (pkt.getHeader().session_handle != 0)
    {
      cerr << "Warning: Non-zero session handle received: " << pkt.getHeader().session_handle << endl;
    }
    if (pkt.getHeader().status != 0)
    {
      cerr << "Warning: Non-zero status received: " << pkt.getHeader().status << endl;
    }
    if (pkt.getHeader().context[0] != 0 || pkt.getHeader().context[1] != 0)
    {
      cerr << "Warning: Non-zero sender context received: " 
           << pkt.getHeader().context[0] << ", " << pkt.getHeader().context[1] << endl;
    }
    if (pkt.getHeader().options != 0)
    {
      cerr << "Warning: Non-zero options received: " << pkt.getHeader().options << endl;
    }

    CPFPacket payload;
    pkt.getPayloadAs(payload);

    if (payload.getItemCount() < 1)
    {
      cerr << "No items in list identity payload!" << endl;
      return;
    }
    if (payload.getItemCount() > 1)
    {
      cerr << "Warning: more than one item in list identity payload " << payload.getItemCount() << endl;
    }

    if (payload.getItems().at(0).getItemType() != EIP_ITEM_LIST_IDENTITY_RESPONSE)
    {
      cerr << "Error: Payload response received with the wrong item type. Expected: "
        << EIP_ITEM_LIST_IDENTITY_RESPONSE << ", received: " << 
        payload.getItems().at(0).getItemType() << endl;
      return;
    }

    IdentityItemData id;
    payload.getItems().at(0).getDataAs(id);

    cout << "=== Received ID Message ===" << endl;
    cout << "Encapsulation Protocol Version: " << (int)id.encap_protocol_version << endl;
    cout << "Address: " << inet_ntoa(id.sockaddr.sin_addr) << " : " << ntohs(id.sockaddr.sin_port) << endl;
    cout << "Vendor ID: " << (int)id.vendor_id << endl;
    cout << "Device Type: " << (int)id.device_type << endl;
    cout << "Product Code: " << (int)id.product_code << endl;
    cout << "Revision: " << (int)id.revision[0] << "." << (int)id.revision[1] << endl;
    cout << "Status: " << (int)id.status << endl;
    cout << "Serial Number: " << (int)id.serial_number << endl;
    cout << "Product Name: " << id.product_name << endl;
    cout << "State: " << (int)id.state << endl;
  }
  catch (std::length_error e)
  {
    printf("ERROR: Packet too short for identity response\n");
  }
}

void IOScanner::run()
{
  sendListIdentityRequest();
  cout << "Waiting for responses." << endl;
  socket_.get_io_service().run();
}

} // namespace eip
