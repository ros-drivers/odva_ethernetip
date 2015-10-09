/**
Software License Agreement (proprietary)

\file      serializable.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include "os32c/serialization/serializable.h"

namespace eip {
namespace serialization {

Writer& operator<<(Writer& writer, const Serializable& s)
{
  s.serialize(writer);
}

Reader& operator>>(Reader& reader, Serializable& s)
{
  s.deserialize(reader);
}

} // namespace serialization
} // namespace eip
