/**
Software License Agreement (proprietary)

\file      eip_path.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_PATH_H
#define OS32C_EIP_PATH_H

#include <vector>
#include <boost/asio.hpp>
#include "os32c/eip_types.h"

using std::vector;
using std::size_t;

/**
 * Class to handle serialization of the Path data type in EthernetIP.
 * Note that this is not a complete implementation of all path segment types,
 * just the elements needed for the OS32c.
 */
class EIPPath
{
public:

  /**
   * Construct an empty path
   */
  EIPPath();

  /**
   * Add a logical class segment
   * @param class_id ID Number of class to add to path
   */
  void addLogicalClass(EIP_USINT class_id);

  /**
   * Add a logical instance segment
   * @param instance_id ID Number of instance to add to path
   */
  void addLogicalInstance(EIP_USINT instance_id);

  /**
   * Add a logical attribute segment
   * @param attribute_id ID Number of attribute to add to path
   */
  void addLogicalAttribute(EIP_USINT attribute_id);

  /**
   * Serialize the path into the given buffer
   * @param buf buffer into which to serialize the path
   * @return number of bytes written to buffer
   * @throw std::length_error if the buffer is too small to contain the path
   */
  size_t serialize(boost::asio::mutable_buffer buf);

private:
  vector<EIP_USINT> path_buf_;

  /**
   * Helper to put a segment into the current path buffer
   * @param type Type of segment to add
   * @param data Data to add to path
   */
  void addSegment(EIP_USINT type, EIP_USINT data);
};

#endif  // OS32C_EIP_PATH_H
