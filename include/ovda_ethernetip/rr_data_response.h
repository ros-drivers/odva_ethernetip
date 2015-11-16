/**
Software License Agreement (BSD)

\file      rr_data_response.h
\authors   Kareem Shehata <kareem@shehata.ca>
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

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/cpf_item.h"
#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"
#include "ovda_ethernetip/serialization/copy_serializable.h"
#include "ovda_ethernetip/rr_data.h"
#include "ovda_ethernetip/message_router_response.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;
using serialization::copy_serializable;

/**
 * Class to encapsulate an RR Data request specifically
 */
class RRDataResponse : public RRData
{
public:

  /**
   * Get the response code given in the Message Router section of the RR Data Response
   * @return service code value
   */
  EIP_USINT getServiceCode() const
  {
    return response_data_.service;
  }

  /**
   * Get the general status code given in the message router section of the RR Data response
   * @return general status code value
   */
  EIP_USINT getGeneralStatus() const
  {
    return response_data_.general_status;
  }

  /**
   * Get a shared pointer to the additional status given in the RR Data response.
   * If no additional status given (i.e. length 0) then the pointer will be null
   * @return Shared pointer to additional status, or null if none given
   */
  shared_ptr<Serializable> getAdditionalStatus()
  {
    return response_data_.getAdditionalStatus();
  }

  /**
   * Get a shared pointer to the data given in the RR Data response.
   * If no data given (i.e. length 0), then the pointer will be null.
   * @return Shared pointer to response data, or null if none given
   */
  shared_ptr<Serializable> getResponseData()
  {
    return response_data_.getResponseData();
  }

  /**
   * Copy the additional status into the serializable type given. Note that
   * status must be available, make sure to check pointer exists first!
   * @param result Structure into which to copy data
   */
  void getAdditionalStatusAs(Serializable& result)
  {
    copy_serializable(result, *response_data_.getAdditionalStatus());
  }

  /**
   * Copy the response data into the given serializable type. Note that data 
   * must be available, make sure to check pointer exists first!
   * @param result Structure into which to copy data
   */
  void getResponseDataAs(Serializable& result)
  {
    copy_serializable(result, *response_data_.getResponseData());
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
