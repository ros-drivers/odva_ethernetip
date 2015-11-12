/**
Software License Agreement (proprietary)

\file      connection.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "eip/connection.h"

#include <boost/make_shared.hpp>
#include <iostream>

using std::cout;
using std::endl;
using boost::shared_ptr;
using boost::make_shared;

namespace eip {

Connection::Connection(const EIP_CONNECTION_INFO_T& o_to_t, const EIP_CONNECTION_INFO_T& t_to_o)
{
  o_to_t_rpi = o_to_t.rpi;
  t_to_o_rpi = t_to_o.rpi;
  o_to_t_buffer_size = o_to_t.buffer_size;
  t_to_o_buffer_size = t_to_o.buffer_size;
  timeout_tick_size = 6;
  timeout_ticks = 80;
  timeout_multiplyer = 0;
  o_to_t_api = 0;
  t_to_o_api = 0;
  setConnectionPoints(o_to_t.assembly_id, t_to_o.assembly_id);
}

void Connection::setConnectionPoints(EIP_USINT origin, EIP_USINT target)
{
  path_.reset();
  path_.addLogicalClass(0x04);
  path_.addLogicalInstance(1);
  path_.addLogicalConnectionPoint(origin);
  path_.addLogicalConnectionPoint(target);
}

shared_ptr<ForwardOpenRequest> Connection::createForwardOpenRequest()
{
  shared_ptr<ForwardOpenRequest> req = make_shared<ForwardOpenRequest> ();

  req->originator_vendor_id = originator_vendor_id;
  req->originator_sn = originator_sn;
  req->connection_sn = connection_sn;

  req->o_to_t_connection_id = o_to_t_connection_id;
  req->t_to_o_connection_id = t_to_o_connection_id;

  req->timeout_tick_size = timeout_tick_size;
  req->timeout_ticks = timeout_ticks;
  req->timeout_multiplyer = timeout_multiplyer;

  req->o_to_t_rpi = o_to_t_rpi;
  req->t_to_o_rpi = t_to_o_rpi;
  req->conn_type = 1;

  // TODO: need connection size info here
  req->setOriginToTargetParams(o_to_t_buffer_size, false, CONN_PRIORITY_SCHEDULED, CONN_TYPE_P2P, false);
  req->setTargetToOriginParams(t_to_o_buffer_size, true, CONN_PRIORITY_SCHEDULED, CONN_TYPE_P2P, false);

  req->getPath() = path_;
  return req;
}

shared_ptr<ForwardCloseRequest> Connection::createForwardCloseRequest()
{
  shared_ptr<ForwardCloseRequest> req = make_shared<ForwardCloseRequest> ();

  req->originator_vendor_id = originator_vendor_id;
  req->originator_sn = originator_sn;
  req->connection_sn = connection_sn;

  req->timeout_tick_size = timeout_tick_size;
  req->timeout_ticks = timeout_ticks;

  // path is actually to the connection manager, not the path of the assemblies
  // don't really understand why, but the example from Omron does this
  req->getPath() = Path(0x02, 0x01);
  return req;
}

bool Connection::verifyForwardOpenResult(const ForwardOpenSuccess& result)
{
  if (result.connection_sn != connection_sn
    || result.originator_vendor_id != originator_vendor_id
    || result.originator_sn != originator_sn)
  {
    return false;
  }
  if (result.o_to_t_connection_id != o_to_t_connection_id)
  {
    cout << "Replacing our o_to_t connection ID (" << o_to_t_connection_id
      << ") with connection ID provided by target (" 
      << result.o_to_t_connection_id << ")" << endl;
    o_to_t_connection_id = result.o_to_t_connection_id;
  }
  if (result.t_to_o_connection_id != t_to_o_connection_id)
  {
    cout << "Replacing our t_to_o connection ID (" << t_to_o_connection_id
      << ") with connection ID provided by target (" 
      << result.t_to_o_connection_id << ")" << endl;
    t_to_o_connection_id = result.t_to_o_connection_id;
  }
  o_to_t_api = result.o_to_t_api;
  t_to_o_api = result.t_to_o_api;
  return true;
}

bool Connection::verifyForwardCloseResult(const ForwardCloseSuccess& result)
{
  // printf("Comparing Forward Close Result: Expected vs Actual\n");
  // printf("Vendor ID: 0x%08X vs 0x%08X\n", originator_vendor_id, result.originator_vendor_id);
  // printf("Origin SN: 0x%08X vs 0x%08X\n", originator_sn, result.originator_sn);
  printf("Connection SN: 0x%08X vs 0x%08X\n", connection_sn, result.connection_sn);
  return (result.connection_sn == connection_sn
    && result.originator_vendor_id == originator_vendor_id
    && result.originator_sn == originator_sn);
}

} // namespace eip
