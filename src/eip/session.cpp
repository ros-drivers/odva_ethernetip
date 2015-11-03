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

  // send the register session message and get response
  EncapPacket response = sendCommand(reg_msg);

  if (response.getHeader().length != reg_data->getLength())
  {
    cerr << "Warning: Registration message received with wrong size. Expected "
       << reg_data->getLength() << " bytes, received "
       << response.getHeader().length << endl;
  }

  response.getPayloadAs(*reg_data);
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

void Session::send(const Serializable& msg)
{
  std::vector<char> buf(msg.getLength());
  BufferWriter writer(buffer(buf));
  msg.serialize(writer);
  socket_.send(buffer(buf));
}

EncapPacket Session::sendCommand(EncapPacket& req)
{
  cout << "Sending Command" << endl;
  send(req);

  cout << "Waiting for response" << endl;
  size_t n = socket_.receive(buffer(recv_buffer_));  
  cout << "Received response of " << n << " bytes" << endl;

  BufferReader reader(buffer(recv_buffer_, n));
  EncapPacket result;
  result.deserialize(reader);

  if (reader.getByteCount() != n)
  {
    cerr << "Warning: packet received with " << n << 
      " bytes, but only " << reader.getByteCount() << " bytes used" << endl;
  }

  if (!check_packet(result, req.getHeader().command))
  {
    throw std::logic_error("Invalid response received");
  }
  return result;
}

bool Session::check_packet(EncapPacket& pkt, EIP_UINT exp_cmd)
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
  return true;
}

RRDataResponse Session::getSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id)
{
  cout << "Creating RR Data Request for Get Single Attribute" << endl;
  shared_ptr<RRDataRequest> req_data = 
    make_shared<RRDataRequest> (0x0E, class_id, instance_id, attribute_id);
  EncapPacket encap_pkt(EIP_CMD_SEND_RR_DATA, session_id_, req_data);

  // send command and get response
  EncapPacket response = sendCommand(encap_pkt);

  RRDataResponse resp_data;
  response.getPayloadAs(resp_data);
  return resp_data;
}

RRDataResponse Session::setSingleAttribute(EIP_USINT class_id,
  EIP_USINT instance_id, EIP_USINT attribute_id, shared_ptr<Serializable> data)
{
  cout << "Creating RR Data Request for Set Single Attribute" << endl;
  shared_ptr<RRDataRequest> req_data =
    make_shared<RRDataRequest> (0x10, class_id, instance_id, attribute_id, data);
  EncapPacket encap_pkt(EIP_CMD_SEND_RR_DATA, session_id_, req_data);

  // send command and get response
  EncapPacket response = sendCommand(encap_pkt);

  RRDataResponse resp_data;
  response.getPayloadAs(resp_data);
  return resp_data;
}

} // namespace eip
