/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_COMMON
#define H_COMMON

#include <glm/gtc/type_precision.hpp>

#define delete_ptr(p) { delete p; p = nullptr; }
#define delete_array(p) { delete [] p; p = nullptr; }

#define BUFFER_OFFSET(i) ((char *)nullptr + i)

#define uint8   glm::uint8
#define int32   glm::int32
#define uint32  glm::uint32

typedef glm::vec3 Position;

#define PI 3.14159265358979323846

// warning C4482: nonstandard extension used: enum 'VertexArray::Attrib' used in qualified name
#pragma warning(disable : 4482)
// warning C4996: 'fscanf': This function or variable may be unsafe. Consider using fscanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable : 4996)
// warning C4244: conversion from 'double' to 'float', possible loss of data)
#pragma warning(disable : 4244)
// warning C4305: 'argument' : truncation from 'double' to 'float'
#pragma warning(disable : 4305)

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

#define RTFORMAT GL_RGBA16F
#define RTFORMAT2 GL_RGBA
#endif
