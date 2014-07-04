

#ifndef	GL_LIGHT_HPP
#define	GL_LIGHT_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


class	GL_Light
{
public :

  static void	Set_Specular(float x, float y, float z, float w);

  static void	Set_Shininess(float value);

  static void	Set_Diffuse(unsigned int light, float x, float y, float z, float w);

  static void	Set_Linear_Attenuation(unsigned int light, float value);

  static void	Set_Quadratic_Attenuation(unsigned int light, float value);

  static void	Set_Constant_Attenuation(unsigned int light, float value);

  static void	Set_Position(unsigned int light, float x, float y, float z);

public :

  static void	Enable_Light(unsigned int light);
  static void	Disable_Light(unsigned int light);

public :

  static void	Enable_Lighting();
  static void	Disable_Lighting();

};


#endif

