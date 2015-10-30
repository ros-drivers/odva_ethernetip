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

  // verify that all fields are correct
  if (reg_msg.getHeader().command != EIP_CMD_REGISTER_SESSION)
  {
    cerr << "Reply received with wrong command. Expected " 
      << EIP_CMD_REGISTER_SESSION << ", received " << reg_msg.getHeader().command << endl;
    // TODO(kshehata) should probably close the socket here or something similar
    return;
  }
  if (reg_msg.getHeader().session_handle == 0)
  {
    cerr << "Warning: Zero session handle received on registration: " << reg_msg.getHeader().session_handle << endl;
  }
  if (reg_msg.getHeader().status != 0)
  {
    cerr << "Warning: Non-zero status received: " << reg_msg.getHeader().status << endl;
  }
  if (reg_msg.getHeader().context[0] != 0 || reg_msg.getHeader().context[1] != 0)
  {
    cerr << "Warning: Non-zero sender context received: " 
    << reg_msg.getHeader().context[0] << " / " << reg_msg.getHeader().context[1] << endl;
  }
  if (reg_msg.getHeader().options != 0)
  {
    cerr << "Warning: Non-zero options received: " << reg_msg.getHeader().options << endl;
  }
  if (reg_msg.getHeader().length != 4)
  {
    cerr << "Warning: Session registration message has wrong size data. "
      "Expected 4 bytes, got " << reg_msg.getHeader().length << endl;
  }
  else
  {
    reg_msg.getPayloadAs(*reg_data);
    if (reg_data->protocol_version != EIP_PROTOCOL_VERSION)
    {
      cerr << "Error: Wrong Ethernet Industrial Protocol Version. "
        "Expected " << EIP_PROTOCOL_VERSION << " got "
        << reg_data->protocol_version << endl;
      // TODO(kshehata): Fail here
    }
    if (reg_data->options != 0)
    {
      cerr << "Warning: Registration message included non-zero options flags: "
        << reg_data->options << endl;
    }
  }

  session_id_ = reg_msg.getHeader().session_handle;
  cout << "Successfully opened session ID " << session_id_ << endl;
}

void Session::send(const Serializable& msg)
{
  std::vector<char> buf(msg.getLength());
  BufferWriter writer(buffer(buf));
  msg.serialize(writer);
  socket_.send(buffer(buf));
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
