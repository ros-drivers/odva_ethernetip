/**
Software License Agreement (proprietary)

\file      rr_data_response.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_RR_DATA_RESPONSE_H
#define EIP_RR_DATA_RESPONSE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "eip/eip_types.h"
#include "eip/cpf_item.h"
#include "eip/serialization/serializable.h"
#include "eip/serialization/reader.h"
#include "eip/serialization/writer.h"
#include "eip/rr_data.h"
#include "eip/message_router_response.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate an RR Data request specifically
 */
class RRDataResponse : public RRData
{
public:

  EIP_USINT getServiceCode() const
  {
    return response_data_.service;
  }

  EIP_USINT getGeneralStatus() const
  {
    return response_data_.general_status;
  }

  shared_ptr<Serializable> getAdditionalStatus()
  {
    return response_data_.getAdditionalStatus();
  }

  shared_ptr<Serializable> getResponseData()
  {
    return response_data_.getResponseData();
  }

protected:
  /**
   * Cannot current serialize this class
   */
  virtual shared_ptr<Serializable> getData() const
  {
    throw std::logic_error("Not implemented");
  }

  /**
   * Used during deserialization to pass in the item data to be parsed
   */
  virtual void setData(CPFItem& item)
  {
    item.getDataAs(response_data_);
  }

private:
  MessageRouterResponse response_data_;
};

} // namespace eip

#endif  // EIP_RR_DATA_RESPONSE_H
