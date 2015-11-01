/**
Software License Agreement (proprietary)

\file      range_and_reflectance_measurement.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_RANGE_AND_REFLECTANCE_MEASUREMENT_H
#define OS32C_RANGE_AND_REFLECTANCE_MEASUREMENT_H

#include <string>
#include <vector>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"
#include "os32c/measurement_report_header.h"

using std::vector;
using eip::serialization::Serializable;
using eip::serialization::Reader;
using eip::serialization::Writer;

namespace os32c {

/**
 * Data structure and operators for OS32C specific Measurement Report data
 * as defined in the OS32C-DM Ethernet/IP Addendum that includes both
 * range and reflectance data in the same message.
 */
class RangeAndReflectanceMeasurement : public Serializable
{
public:
  MeasurementReportHeader header;
  vector<EIP_UINT> range_data;
  vector<EIP_UINT> reflectance_data;

  /**
   * Size of this message including all measurement data.
   */
  virtual size_t getLength() const
  {
    return header.getLength() + range_data.size() * sizeof(EIP_UINT)
      + reflectance_data.size() * sizeof(EIP_UINT);
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    header.serialize(writer);
    writer.writeBytes(&range_data[0], range_data.size() * sizeof(EIP_UINT));
    writer.writeBytes(&reflectance_data[0], reflectance_data.size() * sizeof(EIP_UINT));
    return writer;
  }

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    deserialize(reader);
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    header.deserialize(reader);
    range_data.resize(header.num_beams);
    reflectance_data.resize(header.num_beams);
    reader.readBytes(&range_data[0], range_data.size() * sizeof(EIP_UINT));
    reader.readBytes(&reflectance_data[0], reflectance_data.size() * sizeof(EIP_UINT));
    return reader;
  }
};

} // namespace os32c

#endif  // OS32C_RANGE_AND_REFLECTANCE_MEASUREMENT_H
