/**
Software License Agreement (proprietary)

\file      session.cpp
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
  socket_->open(hostname, port);

  // create the registration message
  cout << "Creating and sending the registration message" << endl;
  shared_ptr<RegisterSessionData> reg_data = make_shared<RegisterSessionData>();
  EncapPacket reg_msg(EIP_CMD_REGISTER_SESSION, 0, reg_data);

  // send the register session message and get response
  EncapPacket response;
  try
  {
    response = sendCommand(reg_msg);
  }
  catch (std::length_error ex)
  {
    socket_->close();
    cerr << "Could not parse response when registering session: " << ex.what() << endl;
    throw std::runtime_error("Invalid response received registering session");
  }
  catch (std::logic_error ex)
  {
    socket_->close();
    cerr << "Error in registration response: " << ex.what() << endl;
    throw std::runtime_error("Error in registration response");
  }

  if (response.getHeader().length != reg_data->getLength())
  {
    cerr << "Warning: Registration message received with wrong size. Expected "
       << reg_data->getLength() << " bytes, received "
       << response.getHeader().length << endl;
  }

  bool response_valid = false;
  try
  {
    response.getPayloadAs(*reg_data);
    response_valid = true;
  }
  catch (std::length_error ex)
  {
    cerr << "Warning: Registration message too short, ignoring" << endl;
  }
  catch (std::logic_error ex)
  {
    cerr << "Warning: could not parse registration response: " << ex.what() << endl;
  }

  if (response_valid && reg_data->protocol_version != EIP_PROTOCOL_VERSION)
  {
    cerr << "Error: Wrong Ethernet Industrial Protocol Version. "
      "Expected " << EIP_PROTOCOL_VERSION << " got "
      << reg_data->protocol_version << endl;
    socket_->close();
    throw std::runtime_error("Received wrong Ethernet IP Protocol Version on registration");
  }
  if (response_valid && reg_data->options != 0)
  {
    cerr << "Warning: Registration message included non-zero options flags: "
      << reg_data->options << endl;
  }

  session_id_ = response.getHeader().session_handle;
  cout << "Successfully opened session ID " << session_id_ << endl;
}

void Session::close()
{
  // TODO: should close all connections and the IO port
  cout << "Closing session" << endl;
  
  // create the unregister session message
  EncapPacket reg_msg(EIP_CMD_UNREGISTER_SESSION, session_id_);
  socket_->send(reg_msg);

  cout << "Session closed" << endl;

  socket_->close();
  session_id_ = 0;
}

EncapPacket Session::sendCommand(EncapPacket& req)
{
  cout << "Sending Command" << endl;
  socket_->send(req);

  cout << "Waiting for response" << endl;
  size_t n = socket_->receive(buffer(recv_buffer_));  
  cout << "Received response of " << n << " bytes" << endl;

  BufferReader reader(buffer(recv_buffer_, n));
  EncapPacket result;
  result.deserialize(reader);

  if (reader.getByteCount() != n)
  {
    cerr << "Warning: packet received with " << n << 
      " bytes, but only " << reader.getByteCount() << " bytes used" << endl;
  }

  check_packet(result, req.getHeader().command);
  return result;
}

void Session::check_packet(EncapPacket& pkt, EIP_UINT exp_cmd)
{
  // verify that all fields are correct
  if (pkt.getHeader().command != exp_cmd)
  {
    cerr << "Reply received with wrong command. Expected " 
      << exp_cmd << ", received " << pkt.getHeader().command << endl;
    throw std::logic_error("Reply received with wrong command");
  }
  if (session_id_ == 0 && pkt.getHeader().session_handle == 0)
  {
    cerr << "Warning: Zero session handle received on registration: " 
      << pkt.getHeader().session_handle << endl;
    throw std::logic_error("Zero session handle received on registration");
  }
  if (session_id_ != 0 && pkt.getHeader().session_handle != session_id_)
  {
    cerr << "Warning: reply received with wrong session ID. Expected "
      << session_id_ << ", recieved " << pkt.getHeader().session_handle << endl;
    throw std::logic_error("Wrong session ID received for command");
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
}

void Session::getSingleAttributeSerializable(EIP_USINT class_id, EIP_USINT instance_id, 
  EIP_USINT attribute_id, Serializable& result)
{
  shared_ptr<Serializable> no_data;
  RRDataResponse resp_data = sendRRDataCommand(0x0E,
    Path(class_id, instance_id, attribute_id), no_data);

  resp_data.getResponseDataAs(result);
}

void Session::setSingleAttributeSerializable(EIP_USINT class_id,
  EIP_USINT instance_id, EIP_USINT attribute_id, shared_ptr<Serializable> data)
{
  RRDataResponse resp_data = sendRRDataCommand(0x10,
    Path(class_id, instance_id, attribute_id), data);
}

RRDataResponse Session::sendRRDataCommand(EIP_USINT service, const Path& path,
  shared_ptr<Serializable> data)
{
  cout << "Creating RR Data Request" << endl;
  shared_ptr<RRDataRequest> req_data =
    make_shared<RRDataRequest> (service, path, data);
  EncapPacket encap_pkt(EIP_CMD_SEND_RR_DATA, session_id_, req_data);

  // send command and get response
  EncapPacket response;
  try
  {
    response = sendCommand(encap_pkt);
  }
  catch (std::length_error ex)
  {
    cerr << "Response packet to RR command too short: " << ex.what() << endl;
    throw std::runtime_error("Packet response to RR Data Command too short");
  }
  catch (std::logic_error ex)
  {
    cerr << "Invalid response to RR command: " << ex.what() << endl;
    throw std::runtime_error("Invalid packet response to RR Data Command");   
  }

  RRDataResponse resp_data;
  try
  {
    response.getPayloadAs(resp_data);
  }
  catch (std::length_error ex)
  {
    cerr << "Response data to RR command too short: " << ex.what() << endl;
    throw std::runtime_error("Response data to RR Command too short");
  }
  catch (std::logic_error ex)
  {
    cerr << "Invalid data to RR command: " << ex.what() << endl;
    throw std::runtime_error("Invalid data in response to RR command");
  }

  // check that responses are valid
  if (resp_data.getServiceCode() != (service | 0x80))
  {
    cerr << "Warning: Wrong service code returned for RR Data command. Expected: "
      << (int)service << " but received " << (int)resp_data.getServiceCode() << endl;
    // throw std::runtime_error("Wrong service code returned for RR Data command");
  }
  if (resp_data.getGeneralStatus())
  {
    cerr << "RR Data Command failed with status " << (int)resp_data.getGeneralStatus() << endl;
    throw std::runtime_error("RR Data Command Failed");
  }
  return resp_data;
}

int Session::createConnection(const EIP_CONNECTION_INFO_T& o_to_t, 
  const EIP_CONNECTION_INFO_T& t_to_o)
{
  Connection conn(o_to_t, t_to_o);
  conn.originator_vendor_id = my_vendor_id_;
  conn.originator_sn = my_serial_num_;
  conn.connection_sn = next_connection_sn_++;
  conn.o_to_t_connection_id = next_connection_id_++;
  conn.t_to_o_connection_id = next_connection_id_++;

  shared_ptr<ForwardOpenRequest> req = conn.createForwardOpenRequest();
  RRDataResponse resp_data = sendRRDataCommand(0x5B, Path(0x06, 1), req);
  ForwardOpenSuccess result;
  resp_data.getResponseDataAs(result);
  if (!conn.verifyForwardOpenResult(result))
  {
    cerr << "Received invalid response to forward open request" << endl;
    throw std::logic_error("Forward Open Response Invalid");
  }

  connections_.push_back(conn);
  return connections_.size() - 1;
}

void Session::closeConnection(size_t n)
{
  shared_ptr<ForwardCloseRequest> req = connections_[n].createForwardCloseRequest();
  RRDataResponse resp_data = sendRRDataCommand(0x4E, Path(0x06, 1), req);
  ForwardCloseSuccess result;
  resp_data.getResponseDataAs(result);
  if (!connections_[n].verifyForwardCloseResult(result))
  {
    cerr << "Received invalid response to forward close request" << endl;
    throw std::logic_error("Forward Close Response Invalid");
  }
  // remove the connection from the list
  connections_.erase(connections_.begin() + n);
}

CPFPacket Session::receiveIOPacket()
{
  cout << "Receiving IO packet" << endl;
  size_t n = io_socket_->receive(buffer(recv_buffer_));
  cout << "Received IO of " << n << " bytes" << endl;

  BufferReader reader(buffer(recv_buffer_, n));
  CPFPacket result;
  result.deserialize(reader);

  if (reader.getByteCount() != n)
  {
    cerr << "Warning: IO packet received with " << n <<
      " bytes, but only " << reader.getByteCount() << " bytes used" << endl;
  }

  return result;
}

void Session::sendIOPacket(CPFPacket& pkt)
{
  cout << "Sending CPF Packet on IO Socket" << endl;
  io_socket_->send(pkt);
}


} // namespace eip
