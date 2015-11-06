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
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/socket/socket.h"
#include "eip/serialization/serializable.h"
#include "eip/serialization/serializable_primitive.h"
#include "eip/encap_packet.h"
#include "eip/rr_data_response.h"

using std::string;
using boost::shared_ptr;
using boost::make_shared;

namespace eip {

using serialization::Serializable;
using serialization::SerializablePrimitive;
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
   * @throw std::runtime_error on problem opening the connection
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
  EIP_UDINT getSessionID()
  {
    return session_id_;
  }

  /**
   * Get a single attribute from the given class / instance / attribute path
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @param result Serializable that will accept the result
   */
  void getSingleAttributeSerializable(EIP_USINT class_id, EIP_USINT instance_id, 
    EIP_USINT attribute_id, Serializable& result);

  /**
   * Shortcut to get a single attribute as a primitive type
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @return Attribute value from target
   */
  template <typename T>
  T getSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id, T v)
  {
    SerializablePrimitive<T> data;
    getSingleAttributeSerializable(class_id, instance_id, attribute_id, data);
    return data.data;
  }

  /**
   * Set a single attribute from the given class / instance / attribute path
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @param data Data to set the attribute to
   */
  void setSingleAttributeSerializable(EIP_USINT class_id, EIP_USINT instance_id, 
    EIP_USINT attribute_id, shared_ptr<Serializable> data);

  /**
   * Shortcut to set a single attribute from a primitive type
   * @param class_id Class ID for the path to get
   * @param instance_id Instance ID number for the path to get
   * @param attribute_id Attribute ID number for the path to get
   * @param v Value to set attribute to
   */
  template <typename T>
  void setSingleAttribute(EIP_USINT class_id, EIP_USINT instance_id, 
    EIP_USINT attribute_id, T v)
  {
    shared_ptr< SerializablePrimitive<T> > data = 
      make_shared< SerializablePrimitive<T> > (v);
    setSingleAttributeSerializable(class_id, instance_id, attribute_id, data);
  }

private:
  shared_ptr<Socket> socket_;
  EIP_UDINT session_id_;
  EIP_BYTE recv_buffer_[4*1024];
  
  /**
   * Helper to check a returned encapsulation packet based on an expected command.
   * Note that there are some warnings given that do not produce an exception
   * @throws std::logic_error if problems are found with the response
   */
  void check_packet(EncapPacket& pkt, EIP_UINT exp_cmd);

  /**
   * Helper to send a command to the target as encapsulation packet and get 
   * the response, also as an encapsulation packet
   * @param req Encapsulation packet to send
   * @return Encapsulation packet received in response
   */
  EncapPacket sendCommand(EncapPacket& req);

  /**
   * Helper to send an RR Data command to the target
   * @param service Service code to send
   * @param class_id Class ID for the path
   * @param instance_id Instance ID number for the path
   * @param attribute_id Attribute ID number for the path
   * @param data Data to include. If null, then no data sent
   * @return RR Data Response
   */
  RRDataResponse sendRRDataCommand(EIP_USINT service, EIP_USINT class_id,
    EIP_USINT instance_id, EIP_USINT attribute_id, shared_ptr<Serializable> data);
};

} // namespace eip

#endif  // EIP_SESSION_H
