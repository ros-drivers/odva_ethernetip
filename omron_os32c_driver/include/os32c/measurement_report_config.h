/**
Software License Agreement (proprietary)

\file      measurement_report_config.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_MEASUREMENT_REPORT_CONFIG_H
#define OS32C_MEASUREMENT_REPORT_CONFIG_H

#include <string>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"

using std::string;
using eip::serialization::Serializable;
using eip::serialization::Reader;
using eip::serialization::Writer;

namespace os32c {

/**
 * Data structure and operators for OS32C specific Measurement Report Header
 * as defined in the OS32C-DM Ethernet/IP Addendum
 */
class MeasurementReportConfig : public Serializable
{
public:
  EIP_UINT sequence_num;
  EIP_UDINT trigger;
  EIP_UINT range_report_format;
  EIP_UINT reflectivity_report_format;
  EIP_BYTE beam_selection_mask[88];

  MeasurementReportConfig() : sequence_num(1), trigger(3),
    range_report_format(1), reflectivity_report_format(2)
  {
    memset(beam_selection_mask, 0, sizeof(beam_selection_mask));
  }

  /**
   * From OS32C-DM Ethernet/IP addendum, header is always 56 bytes
   */
  virtual size_t getLength() const
  {
    return 110;
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    EIP_UINT reserved = 0;
    writer.write(sequence_num);
    writer.write(trigger);
    writer.write(range_report_format);
    writer.write(reflectivity_report_format);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.writeBytes(beam_selection_mask, sizeof(beam_selection_mask));
    return writer;
  }

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    return deserialize(reader);
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader)
  {
    reader.read(sequence_num);
    reader.read(trigger);
    reader.read(range_report_format);
    reader.read(reflectivity_report_format);
    reader.skip(12);
    reader.readBytes(beam_selection_mask, sizeof(beam_selection_mask));
    return reader;
  }
};

} // namespace os32c

#endif  // OS32C_MEASUREMENT_REPORT_CONFIG_H
