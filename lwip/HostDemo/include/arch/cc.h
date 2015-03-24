#pragma once

#define LWIP_PLATFORM_ASSERT(x)

#define BYTE_ORDER LITTLE_ENDIAN
#define LWIP_ERR_T 							__int32

#define PACK_STRUCT_USE_INCLUDES	1
#define PACK_STRUCT_STRUCT

typedef __int8            s8_t;
typedef __int16           s16_t;
typedef __int32           s32_t;
typedef __int64           s64_t;
typedef unsigned __int8   u8_t;
typedef unsigned __int16  u16_t;
typedef unsigned __int32  u32_t;
typedef unsigned __int64  u64_t;

typedef unsigned __int32  mem_ptr_t;

#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u" 
#define S32_F "d" 
#define X32_F "x"
#define SZT_F "u" 

