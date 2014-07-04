
#ifndef NORMALLOOKUPTABLE_HH
#define NORMALLOOKUPTABLE_HH

#include <GL/gl.h>

#include "../Math/Mathlib.hpp"


typedef GLfloat vec3_t[3];


class NormalLookupTable
{
public:

  NormalLookupTable();

  const vec3_t	*operator [] (int i) const;

private:

  vec3_t	_normal[256][256];

};

#endif
