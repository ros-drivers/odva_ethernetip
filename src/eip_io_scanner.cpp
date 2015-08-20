/**
Software License Agreement (proprietary)

\file      eip_io_scanner.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/bind.hpp>

#include "os32c/eip_io_scanner.h"
#include "os32c/eip_types.h"
#include "os32c/eip_encap_pkt.h"
#include "os32c/eip_common_pkt.h"
#include "os32c/eip_identity_item.h"

using namespace boost::asio;
using boost::asio::ip::udp;
using std::cerr;
using std::cout;
using std::endl;

EIPIOScanner::EIPIOScanner(io_service& io_service, string hostname)
    : socket_(io_service), hostname_(hostname)
{
  cout << "Opening UDP socket... ";
  socket_.open(udp::v4());
  socket_.async_receive_from(buffer(recv_buf_), device_endpoint_,
    boost::bind(&EIPIOScanner::handleListIdentityResponse, this, 
      boost::asio::placeholders::error, 
      boost::asio::placeholders::bytes_transferred));
  cout << "done." << endl;
}

void EIPIOScanner::sendListIdentityRequest()
{
  cout << "Sending List Identity Request... ";
  udp::resolver r(socket_.get_io_service());
  udp::resolver::query q(udp::v4(), hostname_, "44818");
  udp::endpoint receiver_endpoint = *r.resolve(q);

  EIPEncapPkt pkt(EIP_CMD_LIST_IDENTITY);
  boost::array<EIP_BYTE, 128> data;
  mutable_buffer send_buf = buffer(data);
  size_t length = pkt.serialize(send_buf);
  socket_.send_to(buffer(send_buf, length), receiver_endpoint);
  cout << "done." << endl;
}

void EIPIOScanner::handleListIdentityResponse(const boost::system::error_code& ec,
  std::size_t num_bytes)
{
  if (ec)
  {
    cerr << "Error receiving list identity response message" << endl;
    return;
  }

  try
  {
    EIPEncapPkt pkt;
    std::size_t n = pkt.deserialize(buffer(recv_buf_, num_bytes));
    if (n != num_bytes)
    {
      cerr << "Warning: packet received with " << num_bytes << 
        " bytes, but only " << n << " bytes used" << endl;
    }

    if (pkt.getCommand() != EIP_CMD_LIST_IDENTITY)
    {
      cerr << "Reply received with wrong command. Expected " 
        << EIP_CMD_LIST_IDENTITY << ", received " << pkt.getCommand() << endl;
      return;
    }
    if (pkt.getSessionHandle() != 0)
    {
      cerr << "Warning: Non-zero session handle received: " << pkt.getSessionHandle() << endl;
    }
    if (pkt.getStatus() != 0)
    {
      cerr << "Warning: Non-zero status received: " << pkt.getStatus() << endl;
    }
    if (pkt.getSenderContext() != 0)
    {
      cerr << "Warning: Non-zero sender context received: " << pkt.getSenderContext() << endl;
    }
    if (pkt.getOptions() != 0)
    {
      cerr << "Warning: Non-zero options received: " << pkt.getOptions() << endl;
    }

    EIPCommonPkt payload;
    payload.deserialize(pkt.getData());

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

    EIPIdentityItem id;
    id.deserialize(payload.getItems().at(0).getItemData());

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

void EIPIOScanner::run()
{
  sendListIdentityRequest();
  cout << "Waiting for responses." << endl;
  socket_.get_io_service().run();
}