/**
Software License Agreement (proprietary)

\file      eip_serialization_helpers.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#define WRITE_BIN(ost, v) ost.write((char*)&v, sizeof(v))
#define READ_BIN(ist, v) ist.read((char*)&v, sizeof(v))
