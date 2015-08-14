/**
Software License Agreement (proprietary)

\file      eip_types.cpp
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#include <stdint.h>

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

typedef struct
{
  EIP_UINT  command;
  EIP_UINT  length;
  EIP_UDINT session_handle;
  EIP_DWORD status;
  EIP_DWORD context[2];
  EIP_DWORD options;
} EIP_ENCAP_HEADER;
