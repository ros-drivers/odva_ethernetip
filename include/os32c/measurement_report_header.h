/**
Software License Agreement (proprietary)

\file      measurement_report_header.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_MEASUREMENT_REPORT_HEADER_H
#define OS32C_MEASUREMENT_REPORT_HEADER_H

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
class MeasurementReportHeader : public Serializable
{
public:
  EIP_UDINT scan_count;
  EIP_UDINT scan_rate;
  EIP_UDINT scan_timestamp;
  EIP_UDINT scan_beam_period;
  EIP_UINT machine_state;
  EIP_UINT machine_stop_reasons;
  EIP_UINT active_zone_set;
  EIP_WORD zone_inputs;
  EIP_WORD detection_zone_status;
  EIP_WORD output_status;
  EIP_WORD input_status;
  EIP_UINT display_status;
  EIP_UINT non_safety_config_checksum;
  EIP_UINT safety_config_checksum;
  EIP_UINT range_report_format;
  EIP_UINT refletivity_report_format;
  EIP_UINT num_beams;

  /**
   * From OS32C-DM Ethernet/IP addendum, header is always 56 bytes
   */
  virtual size_t getLength() const
  {
    return 56;
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
    writer.write(scan_count);
    writer.write(scan_rate);
    writer.write(scan_timestamp);
    writer.write(scan_beam_period);
    writer.write(machine_state);
    writer.write(machine_stop_reasons);
    writer.write(active_zone_set);
    writer.write(zone_inputs);
    writer.write(detection_zone_status);
    writer.write(output_status);
    writer.write(input_status);
    writer.write(display_status);
    writer.write(non_safety_config_checksum);
    writer.write(safety_config_checksum);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(reserved);
    writer.write(range_report_format);
    writer.write(refletivity_report_format);
    writer.write(reserved);
    writer.write(num_beams);
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
    reader.read(scan_count);
    reader.read(scan_rate);
    reader.read(scan_timestamp);
    reader.read(scan_beam_period);
    reader.read(machine_state);
    reader.read(machine_stop_reasons);
    reader.read(active_zone_set);
    reader.read(zone_inputs);
    reader.read(detection_zone_status);
    reader.read(output_status);
    reader.read(input_status);
    reader.read(display_status);
    reader.read(non_safety_config_checksum);
    reader.read(safety_config_checksum);
    reader.skip(6*sizeof(EIP_UINT));
    reader.read(range_report_format);
    reader.read(refletivity_report_format);
    reader.skip(sizeof(EIP_UINT));
    reader.read(num_beams);
    return reader;
  }
};

} // namespace os32c

#endif  // OS32C_MEASUREMENT_REPORT_HEADER_H
