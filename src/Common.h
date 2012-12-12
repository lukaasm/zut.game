/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_COMMON
#define H_COMMON

#include <glm/gtc/type_precision.hpp>

#define delete_ptr(p) { delete p; p = nullptr; }
#define delete_array(p) { delete [] p; p = nullptr; }

#define BUFFER_OFFSET(i) ((char *)nullptr + i)

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0
#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1
#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2
#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3
#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4

#define uint8   glm::uint8
#define int32   glm::int32
#define uint32  glm::uint32

typedef glm::vec3 Position;

// warning C4482: nonstandard extension used: enum 'VertexArray::Attrib' used in qualified name
#pragma warning(disable : 4482)
// warning C4996: 'fscanf': This function or variable may be unsafe. Consider using fscanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable : 4996)

#endif
