/**
Software License Agreement (proprietary)

\file      path.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_PATH_H
#define EIP_PATH_H

#include <vector>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"

using std::vector;
using std::size_t;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to handle serialization of the Path data type in EthernetIP.
 * Note that this is not a complete implementation of all path segment types,
 * just the elements needed for the OS32c.
 */
class Path
{
public:

  /**
   * Construct an empty path
   */
  Path();

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
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const;

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const;

  /**
   * Not actually implemented
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    throw std::logic_error("Not implemented");
  }

  /**
   * Not actually implemented
   */
  virtual Reader& deserialize(Reader& reader)
  {
    throw std::logic_error("Not implemented");
  }

private:
  vector<EIP_USINT> path_buf_;

  /**
   * Helper to put a segment into the current path buffer
   * @param type Type of segment to add
   * @param data Data to add to path
   */
  void addSegment(EIP_USINT type, EIP_USINT data);
};

} // namespace eip

#endif  // EIP_PATH_H
