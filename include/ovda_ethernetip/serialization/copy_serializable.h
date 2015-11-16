/**
Software License Agreement (BSD)

\file      copy_serializable.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_COPY_SERIALIZABLE_H
#define EIP_COPY_SERIALIZABLE_H

#include "ovda_ethernetip/serialization/serializable.h"

namespace eip {
namespace serialization {

void copy_serializable(Serializable& dst, const Serializable& src);

} // namespace serialization
} // namespace eip
#endif  // EIP_COPY_SERIALIZABLE_H
