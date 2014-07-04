//
// FreeFlyCamera.cpp for FreeFlyCamera in /home/bouche_k//projetPerso/sfmlProject/CamOpenGL
// 
// Made by guillaume bouchet
// Login   <bouche_k@epitech.net>
// 
// Started on  Tue Dec 28 20:02:14 2010 guillaume bouchet
// Last update Sun Jun 12 10:46:38 2011 Barbie
//

#include "FreeFlyCamera.h"


#include <iostream>

#include "Game.hh"

FreeFlyCamera::FreeFlyCamera(sf::RenderWindow& App, sf::Vector3f Position)
  : m_App(App)
{
  m_Position = Position;
  m_speed = 40.0f;
  m_sensivity = 0.2;
  m_phi = 0;
  m_theta = 0;
  m_Active = true;
  VectorsFromAngles();

  // // m_App.ShowMouseCursor(false);
  // m_App.SetCursorPosition(m_App.GetWidth() / 2,
  // 			  m_App.GetHeight() / 2);
  // m_Last_Mouse.x = m_App.GetWidth() / 2;
  // m_Last_Mouse.y = m_App.GetHeight() / 2;
}

void		FreeFlyCamera::Control()
{
  if (!m_Active)
    return;
  m_App.SetCursorPosition(m_App.GetWidth() / 2,
			  m_App.GetHeight() / 2);
  m_Last_Mouse.x = m_App.GetWidth() / 2;
  m_Last_Mouse.y = m_App.GetHeight() / 2;

  const sf::Input& Input = m_App.GetInput();

  if (Input.IsKeyDown(sf::Key::A))
    m_Position -= m_Left * m_speed;
  else if (Input.IsKeyDown(sf::Key::D))
    m_Position += m_Left * m_speed;

  if (Input.IsKeyDown(sf::Key::W))
    m_Position +=  m_Forward * m_speed;
  else if (Input.IsKeyDown(sf::Key::S))
    m_Position -=  m_Forward * m_speed;

  {
    while (m_Position.x < 0.0f)
      m_Position.x += WORLD_SIZEX;
    while (m_Position.x > WORLD_SIZEX)
      m_Position.x -= WORLD_SIZEX;

    while (m_Position.y < 0.0f)
      m_Position.y += WORLD_SIZEY;
    while (m_Position.y > WORLD_SIZEY)
      m_Position.y -= WORLD_SIZEY;

    while (m_Position.z < 0.0f)
      m_Position.z += WORLD_SIZEZ;
    while (m_Position.z > WORLD_SIZEZ)
      m_Position.z -= WORLD_SIZEZ;
  }

  m_Target = m_Position + m_Forward;
}

void		FreeFlyCamera::MouseMotion(sf::Event event)
{
  if (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Space)
    SetActive(!m_Active);
  else if (event.Type == sf::Event::LostFocus)
    SetActive(false);
  else if (m_Active && event.Type == sf::Event::MouseMoved)
    {
      const sf::Input& Input = m_App.GetInput();

      m_theta -= (Input.GetMouseX() - m_Last_Mouse.x) * m_sensivity;
      m_phi   -= (Input.GetMouseY() - m_Last_Mouse.y) * m_sensivity;
      m_Last_Mouse.x = Input.GetMouseX();
      m_Last_Mouse.y = Input.GetMouseY();
      VectorsFromAngles();
    }
  else if (m_Active && event.Type == sf::Event::MouseWheelMoved)
    {
      if (event.MouseWheel.Delta == 1)
	m_Position.z -= m_speed;
      else
	m_Position.z += m_speed;
    }
  else if (event.Type == sf::Event::Resized)
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(70, static_cast<double>(this->m_App.GetWidth()) / this->m_App.GetHeight(), 0.0001,10000.0);
      glViewport(0,0, event.Size.Width, event.Size.Height);
    }
}

void		FreeFlyCamera::VectorsFromAngles()
{
  sf::Vector3f Up(0,0,1);

  if (m_phi >= 89)
    m_phi = 89;
  else if (m_phi <= -89)
    m_phi = -89;
  if (m_theta > 360)
    m_theta -= 360;
  else if (m_theta < 0)
    m_theta += 360;

  double Rayon2 = cos((m_phi-90) * M_PI / 180);
  Up.z = sin((m_phi-90) * M_PI / 180);
  Up.x = Rayon2 * cos(m_theta * M_PI / 180);
  Up.y = Rayon2 * sin(m_theta * M_PI / 180);

  double Rayon = cos(m_phi * M_PI / 180);
  m_Forward.z = sin(m_phi * M_PI / 180);
  m_Forward.x = Rayon * cos(m_theta * M_PI / 180);
  m_Forward.y = Rayon * sin(m_theta * M_PI / 180);

  m_Left.x = Up.y * m_Forward.z - Up.z * m_Forward.y;
  m_Left.y = Up.z * m_Forward.x - Up.x * m_Forward.z;
  m_Left.z = Up.x * m_Forward.y - Up.y * m_Forward.x;

  m_Target = m_Position + m_Forward;
}

void		FreeFlyCamera::Look()
{


  // float	phi = asin(m_Forward.z) * 180.0f / M_PI;
  // float	rayon = cos(m_phi * M_PI / 180.0f);
  // float	theta = ((acosf(m_Forward.x / rayon) * 180.0f / M_PI));
  // if (m_Forward.y < 0.0f)
  //   theta = 360 - theta;

  // std::cout << std::endl;
  // std::cout << m_phi << " = " << phi << std::endl;
  // std::cout << m_theta << " = " << theta << std::endl;


  gluLookAt(m_Position.x, m_Position.y, m_Position.z,
	    m_Target.x, m_Target.y, m_Target.z,
	    0,0,1);
}

sf::Vector3f	FreeFlyCamera::GetPosition()
{
  return (m_Position);
}

void		FreeFlyCamera::SetActive(bool Active)
{
  m_Active = Active;
  if ((m_Active = Active) == true)
    m_App.ShowMouseCursor(false);
  else
    m_App.ShowMouseCursor(true);
}

bool		FreeFlyCamera::GetActive()
{
  return (m_Active);
}
