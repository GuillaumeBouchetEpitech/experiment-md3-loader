

#include "Texture1D.hh"


Texture1D::Texture1D()
{
  for (int i = 0; i < 4; i++)
    shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = 0.25f;

  //   shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = 0.0f;
  // for (int i = 0; i < 4; i++)
  //   shaderData[i][1] = 1.0f;

  for (int i = 4; i < 14; i++)
    shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = 0.5f;

  for (int i = 14; i < 32; i++)
    shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = 1.0f;

  glGenTextures( 1, &shaderTexture );
}

Texture1D::~Texture1D()
{
  glDeleteTextures( 1, &shaderTexture );
}

void	Texture1D::Bind() const
{
  glBindTexture( GL_TEXTURE_1D, shaderTexture );
  glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 32, 0, GL_RGB , GL_FLOAT, shaderData );
}
