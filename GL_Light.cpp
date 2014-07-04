

#include "GL_Light.hpp"


void	GL_Light::Set_Specular(float x, float y, float z, float w)
{
  GLfloat	v4[4];

  v4[0] = x;
  v4[1] = y;
  v4[2] = z;
  v4[3] = w;

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, v4);
}

void	GL_Light::Set_Shininess(float value)
{
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, value);
}

void	GL_Light::Set_Diffuse(unsigned int light, float x, float y, float z, float w)
{
  if (light >= 8)
    return;

  GLfloat	v4[4];

  v4[0] = x;
  v4[1] = y;
  v4[2] = z;
  v4[3] = w;

  glLightfv(GL_LIGHT0 + light, GL_DIFFUSE, v4);
}

void	GL_Light::Set_Linear_Attenuation(unsigned int light, float value)
{
  if (light >= 8)
    return;

  glLightf(GL_LIGHT0 + light, GL_LINEAR_ATTENUATION, value);
}

void	GL_Light::Set_Quadratic_Attenuation(unsigned int light, float value)
{
  if (light >= 8)
    return;

  glLightf(GL_LIGHT0 + light, GL_QUADRATIC_ATTENUATION, value);
}

void	GL_Light::Set_Constant_Attenuation(unsigned int light, float value)
{
  if (light >= 8)
    return;

  glLightf(GL_LIGHT0 + light, GL_CONSTANT_ATTENUATION, value);
}

void	GL_Light::Set_Position(unsigned int light, float x, float y, float z)
{
  if (light >= 8)
    return;

  float	v3[4];

  v3[0] = x;
  v3[1] = y;
  v3[2] = z;
  v3[3] = 1;

  glLightfv(GL_LIGHT0 + light, GL_POSITION, v3);
}

//////////////////////////////////////////

void	GL_Light::Enable_Light(unsigned int light)
{
  if (light >= 8)
    return;

  glEnable(GL_LIGHT0 + light);
}

void	GL_Light::Disable_Light(unsigned int light)
{
  if (light >= 8)
    return;

  glDisable(GL_LIGHT0 + light);
}

//////////////////////////////////////////

void	GL_Light::Enable_Lighting()
{
  glEnable(GL_LIGHTING);
}

void	GL_Light::Disable_Lighting()
{
  glDisable(GL_LIGHTING);
}


