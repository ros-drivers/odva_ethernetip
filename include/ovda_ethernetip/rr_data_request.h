/**
Software License Agreement (BSD)

\file      rr_data_request.h
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

#ifndef OVDA_ETHERNETIP_RR_DATA_REQUEST_H
#define OVDA_ETHERNETIP_RR_DATA_REQUEST_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/cpf_item.h"
#include "ovda_ethernetip/serialization/serializable.h"
#include "ovda_ethernetip/serialization/reader.h"
#include "ovda_ethernetip/serialization/writer.h"
#include "ovda_ethernetip/rr_data.h"
#include "ovda_ethernetip/message_router_request.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

using serialization::Serializable;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate an RR Data request specifically
 */
class RRDataRequest : public RRData
{
public:

  /**
   * Construct an empty RR data request
   */
  RRDataRequest(EIP_USINT service_code = 0)
  {
    mr_data_ = make_shared<MessageRouterRequest>(service_code);
  }

  /**
   * Construct an RR data request for the given path and data
   */
  RRDataRequest(EIP_USINT service_code, const Path& path, shared_ptr<Serializable> data)
  {
    mr_data_ = make_shared<MessageRouterRequest>(service_code);
    mr_data_->getPath() = path;
    mr_data_->setData(data);
  }

  /**
   * Construct an RR data request for the given path and without data
   */
  RRDataRequest(EIP_USINT service_code, const Path& path)
  {
    mr_data_ = make_shared<MessageRouterRequest>(service_code);
    mr_data_->getPath() = path;
  }

  /**
   * Get the service code for this request
   * @param service code value
   */
  EIP_USINT getServiceCode() const
  {
    return mr_data_->service;
  }

  /**
   * Set the service code for this request.
   * @param service_code service value to use
   */
  void setServiceCode(EIP_USINT service_code)
  {
    mr_data_->service = service_code;
  }

  /**
   * Get the path for this request. Can be used to modify the path.
   * @return path reference for this request
   */
  Path& getPath()
  {
    return mr_data_->getPath();
  }

  /**
   * Get a shared pointer to the data used for this request. If no value is
   * to be supplied, pointer is null.
   * @return shared pointer to data to be sent with request
   */
  shared_ptr<Serializable> getMRData()
  {
    return mr_data_->getData();
  }

  /**
   * Set the data to be used for this request. If no data is to be set
   * set to null (i.e. the default value of a shared_ptr).
   * @param data Data to be sent with the request
   */
  void setMRData(shared_ptr<Serializable> data)
  {
    mr_data_->setData(data);
  }

protected:
  /**
   * Return reference to the MR request to send
   */
  virtual shared_ptr<Serializable> getData() const
  {
    return mr_data_;
  }

  /**
   * Cannot currently deserialize this class
   */
  virtual void setData(CPFItem& item)
  {
    throw std::logic_error("Not implemented");
  }

private:
  shared_ptr<MessageRouterRequest> mr_data_;
};

} // namespace eip

#endif  // OVDA_ETHERNETIP_RR_DATA_REQUEST_H
