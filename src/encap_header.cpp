/**
Software License Agreement (BSD)

\file      encap_header.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "ovda_ethernetip/encap_header.h"


namespace eip {

using serialization::Reader;
using serialization::Writer;


Writer& EncapHeader::serialize(Writer& writer) const
{
  writer.write(command);
  writer.write(length);
  writer.write(session_handle);
  writer.write(status);
  writer.write(context);
  writer.write(options);
  return writer;
}

Reader& EncapHeader::deserialize(Reader& reader)
{
  reader.read(command);
  reader.read(length);
  reader.read(session_handle);
  reader.read(status);
  reader.read(context);
  reader.read(options);
  return reader;
}

} // namespace eip
