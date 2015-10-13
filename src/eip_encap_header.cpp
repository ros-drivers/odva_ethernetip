/**
Software License Agreement (proprietary)

\file      eip_encap_header.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/eip_encap_header.h"

Writer& EIPEncapHeader::serialize(Writer& writer) const
{
  writer.write(command);
  writer.write(length);
  writer.write(session_handle);
  writer.write(status);
  writer.write(context);
  writer.write(options);
  return writer;
}

Reader& EIPEncapHeader::deserialize(Reader& reader)
{
  reader.read(command);
  reader.read(length);
  reader.read(session_handle);
  reader.read(status);
  reader.read(context);
  reader.read(options);
  return reader;
}
