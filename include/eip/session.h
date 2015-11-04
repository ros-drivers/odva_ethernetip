/**
Software License Agreement (proprietary)

\file      session.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_SESSION_H
#define EIP_SESSION_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/socket/socket.h"
#include "eip/serialization/serializable.h"
#include "eip/encap_packet.h"
#include "eip/rr_data_response.h"

using std::string;
using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using socket::Socket;

/**
 * Class to handle creating and managing Ethernet/IP Sessions
 */
class Session
{
public:

  /**
   * Construct a session to use the given io_service. Doesn't actually open
   * the port or start a session.
   */
  Session(shared_ptr<Socket> socket) : socket_(socket), session_id_(0) { }

  virtual ~Session();

  /**
   * Open the session by opening the port and requesting a session.
   * @param hostname hostname of the target
   * @param port port to use if other than the standard
   */
  void open(string hostname, string port = "44818");

  /**
   * Close the session by unregistering the session and then closing the port
   */
  void close();

  /**
   * Get the ID number assigned to this session by the target
   * @return session ID number
   */
  EIP_UINT getSessionID()
  {
    return session_id_;
  }

  /**
   * Get a single attribute from the given class / instance / attribute path
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @param response data, including status and data
   */
  RRDataResponse getSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id);

  /**
   * Set a single attribute from the given class / instance / attribute path
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @param data Data to set the attribute to
   * @param response data, including status and data
   */
  RRDataResponse setSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, 
    EIP_USINT attribute_id, shared_ptr<Serializable> data);

private:
  shared_ptr<Socket> socket_;
  EIP_UINT session_id_;
  EIP_BYTE recv_buffer_[4*1024];
  
  /**
   * Helper to check a returned encapsulation packet based on an expected command.
   * Note that there are some warnings given that do not produce a false result.
   * @return true if packet can be used, false if there's a serious error
   */
  bool check_packet(EncapPacket& pkt, EIP_UINT exp_cmd);

  /**
   * Helper to send a command to the target as encapsulation packet and get 
   * the response, also as an encapsulation packet
   * @param req Encapsulation packet to send
   * @return Encapsulation packet received in response
   */
  EncapPacket sendCommand(EncapPacket& req);
};

} // namespace eip

#endif  // EIP_SESSION_H
