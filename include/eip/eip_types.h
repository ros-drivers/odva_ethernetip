/**
Software License Agreement (proprietary)

\file      eip_types.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef OS32C_EIP_TYPES_H
#define OS32C_EIP_TYPES_H

#include <stdint.h>

#define EIP_PROTOCOL_VERSION 1

// EtherNet/IP type defs
typedef bool      EIP_BOOL;   //< 1-bit value
typedef uint8_t   EIP_BYTE;   //< 8-bit string
typedef uint8_t   EIP_USINT;  //< unsigned 8-bit "short" integer
typedef int8_t    EIP_SINT;   //< signed 8-bit "short" integer
typedef uint16_t  EIP_WORD;   //< 16-bit string
typedef uint16_t  EIP_UINT;   //< unsigned 16-bit integer
typedef int16_t   EIP_INT;    //< signed 16-bit integer
typedef uint32_t  EIP_DWORD;  //< 32-bit string
typedef uint32_t  EIP_UDINT;  //< 32-bit unsigned integer
typedef int32_t   EIP_DINT;   //< 32-bit signed integer

typedef enum
{
  EIP_BOOL_CODE  = 0xC1,
  EIP_BYTE_CODE  = 0xD1,
  EIP_USINT_CODE = 0xC6,
  EIP_SINT_CODE  = 0xC2,
  EIP_WORD_CODE  = 0xD2,
  EIP_UINT_CODE  = 0xC7,
  EIP_INT_CODE   = 0xC3,
  EIP_DWORD_CODE = 0xD3,
  EIP_UDINT_CODE = 0xC8,
  EIP_DINT_CODE  = 0xC4,
} EIP_TYPE_CODE;

typedef enum
{
  EIP_CMD_NOP                 = 0x0000,
  EIP_CMD_LIST_SERVICES       = 0x0004,
  EIP_CMD_LIST_IDENTITY       = 0x0063,
  EIP_CMD_LIST_INTERFACES     = 0x0064,
  EIP_CMD_REGISTER_SESSION    = 0x0065,
  EIP_CMD_UNREGISTER_SESSION  = 0x0066,
  EIP_CMD_SEND_RR_DATA        = 0x006F,
  EIP_CMD_SEND_UNIT_DATA      = 0x0070,
  EIP_CMD_INDICATE_STATUS     = 0x0072,
  EIP_CMD_CANCEL              = 0x0073,
} EIP_CMD_T;

typedef enum
{
  EIP_STATUS_SUCCESS          = 0x0000,
  EIP_STATUS_INVALID_CMD      = 0x0001,
  EIP_STATUS_OUT_OF_MEMORY    = 0x0002,
  EIP_STATUS_INVALI_DATA      = 0x0003,
  EIP_STATUS_INVALID_SESSION  = 0x0064,
  EIP_STATUS_INVALID_LENGTH   = 0x0065,
  EIP_STATUS_INVALID_VERSION  = 0x0069,
} EIP_STATUS_T;

typedef enum
{
  EIP_ITEM_NULL                       = 0x0000,
  EIP_ITEM_LIST_IDENTITY_RESPONSE     = 0x000C,
  EIP_ITEM_CONNECTION_BASED           = 0x00A1,
  EIP_ITEM_CONNECTED_TRANSPORT_PACKET = 0x00B1,
  EIP_ITEM_UNCONNECTED_MESSAGE        = 0x00B2,
  EIP_ITEM_LIST_SERVICES_RESPONSE     = 0x0100,
  EIP_ITEM_SOCKADDR_INFO_ORIGINATOR   = 0x8000,
  EIP_ITEM_SOCKADDR_INFO_TARGET       = 0x8001,
  EIP_ITEM_SEQ_ADDRESS                = 0x8002,
} EIP_ITEM_T;

typedef struct
{
  EIP_UINT  command;
  EIP_UINT  length;
  EIP_UDINT session_handle;
  EIP_DWORD status;
  EIP_DWORD context[2];
  EIP_DWORD options;
} EIP_ENCAP_HEADER;

#endif  // OS32C_EIP_TYPES_H
