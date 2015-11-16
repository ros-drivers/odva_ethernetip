/**
Software License Agreement (BSD)

\file      rr_data.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_RR_DATA_H
#define EIP_RR_DATA_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/cpf_item.h"
#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"

using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate an RR Data request or reply. Note that this is intended
 * to be used within the payload of a EncapPacket.
 */
class RRData : public Serializable
{
public:

  EIP_UDINT interface_handle;
  EIP_UINT timeout;

  /**
   * Construct an empty RR data holder
   */
  RRData() : interface_handle(0), timeout(0) { }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    size_t result = sizeof(interface_handle) + sizeof(timeout) + 5 * sizeof(EIP_UINT);
    if (getData())
    {
      result += getData()->getLength();
    }
    return result;
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const;

  /**
   * Extra length information is not relevant in this context. Same as deserialize(reader)
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    deserialize(reader);
    return reader;
  }

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader);

protected:
  /**
   * Serialize data for the specific implementation. Default is null
   * @return Shared pointer to the serializable of the data to send
   */
  virtual shared_ptr<Serializable> getData() const
  {
    return shared_ptr<Serializable>();
  }

  /**
   * Deserialize the data for this specific implementation
   * @param item the CPF item that was deserialized, copy data from that
   */
  virtual void setData(CPFItem& item) { }
};

} // namespace eip

#endif  // EIP_RR_DATA_H
