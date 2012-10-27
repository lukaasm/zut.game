/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_COMMON
#define H_COMMON

#include <glm/gtc/type_precision.hpp>
using namespace glm;

#define delete_ptr(p) { delete p; p = nullptr; }
#define delete_array(p) { delete [] p; p = nullptr; }

#define BUFFER_OFFSET(i) ((char *)nullptr + i)

#endif
