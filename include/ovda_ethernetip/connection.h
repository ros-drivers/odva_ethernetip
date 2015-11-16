/**
Software License Agreement (proprietary)

\file      connection.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_CONNECTION_H
#define EIP_CONNECTION_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/path.h"
#include "ovda_ethernetip/forward_open_request.h"
#include "ovda_ethernetip/forward_open_success.h"
#include "ovda_ethernetip/forward_close_request.h"
#include "ovda_ethernetip/forward_close_success.h"

using boost::shared_ptr;

namespace eip {

/**
 * Class to encapsulate the EIP concept of a connection.
 */
class Connection
{
public:

  EIP_UINT originator_vendor_id;
  EIP_UDINT originator_sn;
  EIP_UINT connection_sn;

  EIP_UDINT o_to_t_connection_id;
  EIP_UDINT t_to_o_connection_id;

  EIP_BYTE timeout_tick_size;
  EIP_USINT timeout_ticks;
  EIP_USINT timeout_multiplyer;

  EIP_UDINT o_to_t_rpi;
  EIP_UDINT t_to_o_rpi;
  EIP_UDINT o_to_t_api;
  EIP_UDINT t_to_o_api;

  EIP_UINT o_to_t_buffer_size;
  EIP_UINT t_to_o_buffer_size;

  /**
   * Create a connection instance using the directional values and reasonable defaults
   * @param o_to_t Originator to Target info
   * @param t_to_o Target to Originator info
   */
  Connection(const EIP_CONNECTION_INFO_T& o_to_t, const EIP_CONNECTION_INFO_T& t_to_o);

  /**
   * Get the path in the given message router request
   * @return reference to the current Path
   */
  Path& getPath()
  {
    return path_;
  }

  /**
   * Shortcut to set the path based on connection points
   * @param origin starting connection point
   * @param target ending connection point
   */
  void setConnectionPoints(EIP_USINT origin, EIP_USINT target);

  /**
   * Create the forward open request from the data in this connection object
   */
  shared_ptr<ForwardOpenRequest> createForwardOpenRequest();

  /**
   * Create a forward close request from the data in this connection object
   */
  shared_ptr<ForwardCloseRequest> createForwardCloseRequest();

  /**
   * Verify that the result of a forward open is valid and update accordingly
   * @return false if there are significant errors in the result
   */
  bool verifyForwardOpenResult(const ForwardOpenSuccess& result);

  /**
   * Verify that the result of a forward close is valid and update accordingly
   * @return false if there are signficant errors in the result
   */
  bool verifyForwardCloseResult(const ForwardCloseSuccess& result);

private:
    Path path_;

};

} // namespace eip

#endif  // EIP_CONNECTION_H
