/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_COMMON
#define H_COMMON

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;
typedef signed long     int64;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   uint64;

#define delete_ptr(p) { delete p; p = NULL; }
#define delete_array(p) { delete [] p; p = NULL; }

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif
