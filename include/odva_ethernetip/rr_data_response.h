/**
Software License Agreement (BSD)

\file      rr_data_response.h
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ODVA_ETHERNETIP_RR_DATA_RESPONSE_H
#define ODVA_ETHERNETIP_RR_DATA_RESPONSE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "odva_ethernetip/eip_types.h"
#include "odva_ethernetip/cpf_item.h"
#include "odva_ethernetip/serialization/serializable.h"
#include "odva_ethernetip/serialization/reader.h"
#include "odva_ethernetip/serialization/writer.h"
#include "odva_ethernetip/serialization/copy_serializable.h"
#include "odva_ethernetip/rr_data.h"
#include "odva_ethernetip/message_router_response.h"

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

#endif  // ODVA_ETHERNETIP_RR_DATA_RESPONSE_H
