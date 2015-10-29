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
   * Construct an empty RR data holder
   */
  RRDataRequest(EIP_USINT service_code = 0)
  {
    mr_data_ = make_shared<MessageRouterRequest>(service_code);
  }

  EIP_USINT getServiceCode() const
  {
    return mr_data_->service;
  }

  void setServiceCode(EIP_USINT service_code)
  {
    mr_data_->service = service_code;
  }

  Path& getPath()
  {
    return mr_data_->getPath();
  }

  shared_ptr<Serializable> getData()
  {
    return mr_data_->getData();
  }

  void setData(shared_ptr<Serializable> data)
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

private:
  shared_ptr<MessageRouterRequest> mr_data_;
};

} // namespace eip

#endif  // EIP_RR_DATA_REQUEST_H
