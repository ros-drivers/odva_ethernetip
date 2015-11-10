/**
Software License Agreement (proprietary)

\file      rr_data_request.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_RR_DATA_REQUEST_H
#define EIP_RR_DATA_REQUEST_H

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
#include "eip/message_router_request.h"

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

#endif  // EIP_RR_DATA_REQUEST_H
