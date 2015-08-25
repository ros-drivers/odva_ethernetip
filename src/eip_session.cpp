/**
Software License Agreement (proprietary)

\file      eip_session.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <boost/bind.hpp>

#include "os32c/eip_session.h"
#include "os32c/eip_types.h"
#include "os32c/eip_encap_pkt.h"
#include "os32c/eip_common_pkt.h"
#include "os32c/eip_identity_item.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using std::cerr;
using std::cout;
using std::endl;

EIPSession::EIPSession(io_service& io_service) : socket_(io_service), session_id_(0)
{
}

void EIPSession::open(string hostname, string port)
{
  cout << "Resolving hostname and connecting socket" << endl;
  tcp::resolver resolver(socket_.get_io_service());
  tcp::resolver::query query(hostname, port);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  connect(socket_, endpoint_iterator);

  // create the registration message
  cout << "Creating and sending the registration message" << endl;
  EIPEncapPkt reg_msg(EIP_CMD_REGISTER_SESSION);
  boost::array<EIP_UINT, 2> reg_data;
  reg_data[0] = EIP_PROTOCOL_VERSION;
  reg_data[1] = 0;
  reg_msg.setData(buffer(reg_data));

  // send the register session message
  boost::array<EIP_BYTE, 1024> buf;
  size_t n = reg_msg.serialize(buffer(buf));
  socket_.send(buffer(buf, n));

  cout << "Waiting for response" << endl;
  // receive the response
  n = socket_.receive(buffer(buf));

  cout << "Received response of " << n << " bytes" << endl;

  // TODO(kshehata): Should probably catch exceptions here if the packet is malformed
  size_t n_msg = reg_msg.deserialize(buffer(buf, n));

  if (n_msg != n)
  {
    cerr << "Warning: packet received with " << n << 
      " bytes, but only " << n_msg << " bytes used" << endl;
  }

  // verify that all fields are correct
  if (reg_msg.getCommand() != EIP_CMD_REGISTER_SESSION)
  {
    cerr << "Reply received with wrong command. Expected " 
      << EIP_CMD_REGISTER_SESSION << ", received " << reg_msg.getCommand() << endl;
    // TODO(kshehata) should probably close the socket here or something similar
    return;
  }
  if (reg_msg.getSessionHandle() == 0)
  {
    cerr << "Warning: Zero session handle received on registration: " << reg_msg.getSessionHandle() << endl;
  }
  if (reg_msg.getStatus() != 0)
  {
    cerr << "Warning: Non-zero status received: " << reg_msg.getStatus() << endl;
  }
  if (reg_msg.getSenderContext() != 0)
  {
    cerr << "Warning: Non-zero sender context received: " << reg_msg.getSenderContext() << endl;
  }
  if (reg_msg.getOptions() != 0)
  {
    cerr << "Warning: Non-zero options received: " << reg_msg.getOptions() << endl;
  }
  if (buffer_size(reg_msg.getData()) != 4)
  {
    cerr << "Warning: Session registration message has wrong size data. "
      "Expected 4 bytes, got " << buffer_size(reg_msg.getData()) << endl;
  }
  else
  {
    EIP_UINT* resp_data = buffer_cast<EIP_UINT*>(reg_msg.getData());
    if (resp_data[0] != EIP_PROTOCOL_VERSION)
    {
      cerr << "Error: Wrong Ethernet Industrial Protocol Version. "
        "Expected " << EIP_PROTOCOL_VERSION << " got " << resp_data[0] << endl;
      // TODO(kshehata): Fail here
    }
    if (resp_data[1] != 0)
    {
      cerr << "Warning: Registration message included non-zero options flags: "
        << resp_data[1] << endl;
    }
  }

  session_id_ = reg_msg.getSessionHandle();
  cout << "Successfully opened session ID " << session_id_ << endl;
}


void EIPSession::close()
{
  cout << "Closing session" << endl;
  
  // create the unregister session message
  EIPEncapPkt reg_msg(EIP_CMD_UNREGISTER_SESSION, session_id_);

  // send the unregister session message
  boost::array<EIP_BYTE, 128> buf;
  size_t n = reg_msg.serialize(buffer(buf));
  socket_.send(buffer(buf, n));

  cout << "Session closed" << endl;

  socket_.close();
  session_id_ = 0;
}
