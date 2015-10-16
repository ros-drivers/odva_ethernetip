/**
Software License Agreement (proprietary)

\file      cpf_item.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_CPF_ITEM_H
#define EIP_CPF_ITEM_H

#include <boost/shared_ptr.hpp>

#include "eip/eip_types.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/serialization/serializable.h"

using boost::shared_ptr;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Representation of a Common Packet Format Item. Uses a Serializable reference
 * for the actual data.
 */
class CPFItem : public Serializable
{
public:

  /**
   * Construct null item by default. Anything else requires both the type and 
   * a serializable for the data.
   */
  CPFItem() : item_type_(EIP_ITEM_NULL) { }

  /**
   * Construct a common packet item to represent the given item type and data
   * @param item_type Item type code for this item (see EIP_ITEM_T)
   * @param item_data Data for this item
   */
  CPFItem(EIP_UINT item_type, shared_ptr<Serializable> item_data) 
    : item_type_(item_type), item_data_(item_data) { }

  /**
   * Get the type code for this item
   * @return Item type code (see EIP_ITEM_T)
   */
  EIP_UINT getItemType()
  {
    return item_type_;
  }

  /**
   * Set the type code for this item
   * @param item_type Item type code (see EIP_ITEM_T)
   */
  void setItemType(EIP_UINT item_type)
  {
    item_type_ = item_type;
  }

  /**
   * Get the data portion of this item
   */
  shared_ptr<Serializable> getData()
  {
    return item_data_;
  }

  /**
   * Set the data portion of this item. If a null reference is given, 
   * the item is assumed to be a NullItem
   */
  void setData(shared_ptr<Serializable> data)
  {
    item_data_ = data;
  }

  /**
   * Get the length of the data in this item.
   * @return length in bytes of the data for this item
   */
  virtual EIP_UINT getDataLength() const
  {
    return item_data_ ? item_data_->getLength() : 0;
  }

  /**
   * Get the total length needed to serialize this item. Really just 4 bytes
   * (2 for the type, 2 for length) plus the size of the data itself.
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return 4 + getDataLength();
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const;

  /**
   * Deserialize data from the given reader with the length given
   * @param reader Reader to use for deserialization
   * @param length Length expected for data
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader, size_t length);

  /**
   * Deserialize data from the given reader without length information
   * @param reader Reader to use for deserialization
   * @return the reader again
   * @throw std::length_error if the buffer is overrun while deserializing
   */
  virtual Reader& deserialize(Reader& reader);
private:
  EIP_UINT item_type_;
  shared_ptr<Serializable> item_data_;

  /**
   * Helper to deserialize the item data
   * @param reader the reader to use to deserialize data
   * @param item_length the length in bytes of the data to read
   */
  void deserializeData(Reader& reader, EIP_UINT item_length);
};

} // namespace eip

#endif  // EIP_CPF_ITEM_H
