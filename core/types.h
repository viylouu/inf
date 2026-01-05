#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float  f32;
typedef double f64;


// helper macros

#ifdef INF_DEBUG
#ifndef inf_debug_msg
#include <stdio.h>
#define inf_debug_msg(msg) \
    printf("\033[33m[debug] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, (msg));
#endif

#ifndef inf_debug_fmt
#include <stdio.h>
#define inf_debug_fmt(msg, ...) do { \
    char bufBUTCALLITSOMETHINGELSE[1024]; \
    snprintf(bufBUTCALLITSOMETHINGELSE, sizeof(bufBUTCALLITSOMETHINGELSE), (msg), __VA_ARGS__); \
    printf("\033[33m[debug] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, bufBUTCALLITSOMETHINGELSE); \
} while(0)
#endif
#else
#ifndef inf_debug_msg
#define inf_debug_msg(msg) //
#endif
#ifndef inf_debug_fmt
#define inf_debug_fmt(msg, ...) //
#endif
#endif

#ifndef inf_err_msg
#include <stdio.h>
#define inf_err_msg(msg, ...) \
    printf("\033[1;31m[err] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, (msg));
#endif

#ifndef inf_err_fmt
#include <stdio.h>
#define inf_err_fmt(msg, ...) do { \
    char bufBUTCALLITSOMETHINGELSE[1024]; \
    snprintf(bufBUTCALLITSOMETHINGELSE, sizeof(bufBUTCALLITSOMETHINGELSE), (msg), __VA_ARGS__); \
    printf("\033[1;31m[err] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, bufBUTCALLITSOMETHINGELSE); \
} while(0) 
#endif

#ifndef inf_warn_msg
#include <stdio.h>
#define inf_warn_msg(msg, ...) \
    printf("\033[1;33m[warn] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, (msg));
#endif

#ifndef inf_warn_fmt
#include <stdio.h>
#define inf_warn_fmt(msg, ...) do { \
    char bufBUTCALLITSOMETHINGELSE[1024]; \
    snprintf(bufBUTCALLITSOMETHINGELSE, sizeof(bufBUTCALLITSOMETHINGELSE), (msg), __VA_ARGS__); \
    printf("\033[1;33m[warn] (%s - %d) %s\033[0m\n", __FILE__, __LINE__, bufBUTCALLITSOMETHINGELSE); \
} while(0) 
#endif

#ifndef inf_malloc
#include <stdlib.h>
#define inf_malloc(size) \
    malloc((size))
#endif

#ifndef inf_free
#include <stdlib.h>
#define inf_free(var) do { \
    free((var)); \
    (var) = NULL; \
} while(0)
#endif
