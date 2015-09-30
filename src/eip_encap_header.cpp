/**
Software License Agreement (proprietary)

\file      eip_encap_header.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_encap_header.h"

#define WRITE_BIN(ost, v) ost.write((char*)&v, sizeof(v))
#define READ_BIN(ist, v) ist.read((char*)&v, sizeof(v))


std::ostream& operator<<(std::ostream& ost, const EIPEncapHeader& header)
{
  WRITE_BIN(ost, header.command);
  WRITE_BIN(ost, header.length);
  WRITE_BIN(ost, header.session_handle);
  WRITE_BIN(ost, header.status);
  WRITE_BIN(ost, header.context);
  WRITE_BIN(ost, header.options);
  return ost;
}

std::istream& operator>>(std::istream& ist, EIPEncapHeader& header)
{
  READ_BIN(ist, header.command);
  READ_BIN(ist, header.length);
  READ_BIN(ist, header.session_handle);
  READ_BIN(ist, header.status);
  READ_BIN(ist, header.context);
  READ_BIN(ist, header.options);
  return ist;
}
