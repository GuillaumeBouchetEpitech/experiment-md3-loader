/*
** FreeFlyCamera.h for CamOpenGL in /home/bouche_k//projetPerso/sfmlProject/CamOpenGL
** 
** Made by guillaume bouchet
** Login   <bouche_k@epitech.net>
** 
** Started on  Tue Dec 28 19:57:56 2010 guillaume bouchet
** Last update Fri Jun 10 11:50:53 2011 Barbie
*/

#ifndef FREEFLYCAMERA_H
#define FREEFLYCAMERA_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

class   FreeFlyCamera
{
 private :
  sf::RenderWindow&	m_App;
  sf::Vector3f		m_Position;
  sf::Vector3f		m_Forward;
  sf::Vector3f		m_Left;
  sf::Vector3f		m_Target;
  float			m_speed;
  float			m_sensivity;
  float			m_theta;
  float			m_phi;
  sf::Vector2f		m_Last_Mouse;
  bool			m_Active;

  void		VectorsFromAngles();

 public :
  FreeFlyCamera(sf::RenderWindow& App,
		sf::Vector3f Position = sf::Vector3f(0, 0, 0));
  void		Control();
  void		MouseMotion(sf::Event event);
  void		Look();
  sf::Vector3f	GetPosition();
  sf::Vector3f	GetForward(){return (m_Forward);}
  void		SetActive(bool Active = true);
  bool		GetActive();
};

#endif
