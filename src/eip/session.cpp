/**
Software License Agreement (proprietary)

\file      eip_session.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "eip/serialization/buffer_reader.h"
#include "eip/serialization/buffer_writer.h"
#include "eip/session.h"
#include "eip/eip_types.h"
#include "eip/encap_packet.h"
#include "eip/register_session_data.h"
#include "eip/rr_data_request.h"
#include "eip/rr_data_response.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::shared_ptr;
using boost::make_shared;
using std::cerr;
using std::cout;
using std::endl;

namespace eip {

using serialization::BufferReader;
using serialization::BufferWriter;

Session::~Session()
{
  try
  {
    if (session_id_ != 0)
    {
      close();
    }
  }
  catch (...)
  {
    // can't throw exceptions, but can't do anything either
  }
}

void Session::open(string hostname, string port)
{
  cout << "Resolving hostname and connecting socket" << endl;
  tcp::resolver resolver(socket_.get_io_service());
  tcp::resolver::query query(hostname, port);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  connect(socket_, endpoint_iterator);

  // create the registration message
  cout << "Creating and sending the registration message" << endl;
  shared_ptr<RegisterSessionData> reg_data = make_shared<RegisterSessionData>();
  EncapPacket reg_msg(EIP_CMD_REGISTER_SESSION, 0, reg_data);

  // send the register session message
  send(reg_msg);

  cout << "Waiting for response" << endl;
  // receive the response
  char buf[1024];
  size_t n = socket_.receive(buffer(buf));

  cout << "Received response of " << n << " bytes" << endl;

  // TODO(kshehata): Should probably catch exceptions here if the packet is malformed
  BufferReader reader(buffer(buf, n));
  reg_msg.deserialize(reader);

  if (reader.getByteCount() != n)
  {
    cerr << "Warning: packet received with " << n << 
      " bytes, but only " << reader.getByteCount() << " bytes used" << endl;
  }

  if (!check_packet(reg_msg, EIP_CMD_REGISTER_SESSION, 4))
  {
    return;
  }

  reg_msg.getPayloadAs(*reg_data);
  if (reg_data->protocol_version != EIP_PROTOCOL_VERSION)
  {
    cerr << "Error: Wrong Ethernet Industrial Protocol Version. "
      "Expected " << EIP_PROTOCOL_VERSION << " got "
      << reg_data->protocol_version << endl;
    // TODO(kshehata): Fail here
    return;
  }
  if (reg_data->options != 0)
  {
    cerr << "Warning: Registration message included non-zero options flags: "
      << reg_data->options << endl;
  }

  session_id_ = reg_msg.getHeader().session_handle;
  cout << "Successfully opened session ID " << session_id_ << endl;
}


bool Session::check_packet(EncapPacket& pkt, EIP_UINT exp_cmd, int exp_length)
{
  // verify that all fields are correct
  if (pkt.getHeader().command != exp_cmd)
  {
    cerr << "Reply received with wrong command. Expected " 
      << exp_cmd << ", received " << pkt.getHeader().command << endl;
    return false;
  }
  if (session_id_ == 0 && pkt.getHeader().session_handle == 0)
  {
    cerr << "Warning: Zero session handle received on registration: " 
      << pkt.getHeader().session_handle << endl;
    return false;
  }
  if (session_id_ != 0 && pkt.getHeader().session_handle != session_id_)
  {
    cerr << "Warning: reply received with wrong session ID. Expected "
      << session_id_ << ", recieved " << pkt.getHeader().session_handle << endl;
    return false;
  }
  if (pkt.getHeader().status != 0)
  {
    cerr << "Warning: Non-zero status received: " << pkt.getHeader().status << endl;
  }
  if (pkt.getHeader().context[0] != 0 || pkt.getHeader().context[1] != 0)
  {
    cerr << "Warning: Non-zero sender context received: " 
    << pkt.getHeader().context[0] << " / " << pkt.getHeader().context[1] << endl;
  }
  if (pkt.getHeader().options != 0)
  {
    cerr << "Warning: Non-zero options received: " << pkt.getHeader().options << endl;
  }
  if (exp_length >= 0 && pkt.getHeader().length != exp_length)
  {
    cerr << "Warning: Message received with wrong size. Expected " << exp_length
      << " bytes, received " << pkt.getHeader().length << endl;
    return false;
  }
  return true;
}

void Session::send(const Serializable& msg)
{
  std::vector<char> buf(msg.getLength());
  BufferWriter writer(buffer(buf));
  msg.serialize(writer);
  socket_.send(buffer(buf));
}

RRDataResponse Session::getSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id)
{
  cout << "Creating RR Data Request for Get Single Attribute" << endl;
  shared_ptr<RRDataRequest> req_data = 
    make_shared<RRDataRequest> (0x0E, class_id, instance_id, attribute_id);
  EncapPacket encap_pkt(EIP_CMD_SEND_RR_DATA, session_id_, req_data);
  cout << "Sending RR Data Request" << endl;
  send(encap_pkt);

  cout << "Waiting for response" << endl;
  char buf[4*1024];
  size_t n = socket_.receive(buffer(buf));  
  cout << "Received response of " << n << " bytes" << endl;

  BufferReader reader(buffer(buf, n));
  encap_pkt.deserialize(reader);

  if (reader.getByteCount() != n)
  {
    cerr << "Warning: packet received with " << n << 
      " bytes, but only " << reader.getByteCount() << " bytes used" << endl;
  }

  if (!check_packet(encap_pkt, EIP_CMD_SEND_RR_DATA))
  {
    throw std::logic_error("Invalid response received");
  }

  RRDataResponse resp_data;
  encap_pkt.getPayloadAs(resp_data);
  return resp_data;
}

void Session::close()
{
  cout << "Closing session" << endl;
  
  // create the unregister session message
  EncapPacket reg_msg(EIP_CMD_UNREGISTER_SESSION, session_id_);
  send(reg_msg);

  cout << "Session closed" << endl;

  socket_.close();
  session_id_ = 0;
}

} // namespace eip
