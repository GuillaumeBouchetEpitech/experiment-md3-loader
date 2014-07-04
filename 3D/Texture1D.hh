

#ifndef	TEXTURE1D_HH
#define	TEXTURE1D_HH


#include "GL/gl.h"
#include "GL/glu.h"


class	Texture1D
{
private :

  GLuint	shaderTexture;

public :

  float		shaderData[32][3];

public :

  Texture1D();
  ~Texture1D();

  void	Bind() const;

};


#endif

