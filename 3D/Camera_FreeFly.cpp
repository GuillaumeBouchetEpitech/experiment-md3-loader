
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Camera_FreeFly.hh"


Camera_FreeFly::Camera_FreeFly(sf::RenderWindow& RWindow)
  : m_RWindow(RWindow)
{
  m_Position = sf::Vector3f(0, 0, 0);
  m_speed = 0.0625;
  m_sensivity = 0.1;
  m_phi = 0;
  m_theta = 0;
}

void  Camera_FreeFly::VectorsFromAngles()
{
  if (m_phi >= 89)
    m_phi = 89;
  else if (m_phi <= -89)
    m_phi = -89;
  if (m_theta > 360)
    m_theta -= 360;
  else if (m_theta < 0)
    m_theta += 360;

  double Rayon2 = cos((m_phi - 90) * M_PI / 180);
  m_Up.z = sin((m_phi - 90) * M_PI / 180);
  m_Up.x = Rayon2 * cos(m_theta * M_PI / 180);
  m_Up.y = Rayon2 * sin(m_theta * M_PI / 180);

  double Rayon = cos(m_phi * M_PI / 180);
  m_Forward.z = sin(m_phi * M_PI / 180);
  m_Forward.x = Rayon * cos(m_theta * M_PI / 180);
  m_Forward.y = Rayon * sin(m_theta * M_PI / 180);

  m_Left.x = m_Up.y * m_Forward.z - m_Up.z * m_Forward.y;
  m_Left.y = m_Up.z * m_Forward.x - m_Up.x * m_Forward.z;
  m_Left.z = m_Up.x * m_Forward.y - m_Up.y * m_Forward.x;

  m_Target = m_Position + m_Forward;
}

void	Camera_FreeFly::Initialize()
{
  VectorsFromAngles();

  sf::Vector2u	WSize = m_RWindow.getSize();
  sf::Vector2f	WSizef(WSize.x, WSize.y);
  sf::Vector2i	HWSize(WSize.x / 2, WSize.y / 2);

  // m_RWindow.SetCursorPosition(m_RWindow.GetWidth() / 2, m_RWindow.getSize().y / 2);
  sf::Mouse::setPosition(HWSize, m_RWindow);


  SetActive(true);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // gluPerspective(70, static_cast<double>(this->m_RWindow.GetWidth()) / this->m_RWindow.getSize().y, 1.0,10000.0);
  // glViewport(0,0, this->m_RWindow.GetWidth(), this->m_RWindow.getSize().y);

  gluPerspective(70.0, WSizef.x / WSizef.y, 0.01,100.0);
  glViewport(0,0, WSize.x, WSize.y);

  glMatrixMode(GL_MODELVIEW);
}

void	Camera_FreeFly::Event(const sf::Event& event)
{
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
    SetActive(!m_Active);

  else if (event.type == sf::Event::LostFocus)
    SetActive(false);

  // else if (m_Active && event.type == sf::Event::MouseWheelMoved)
  //   {
  //     if (event.mouseWheel.delta == 1)
  // 	m_Position.z -= m_speed;
  //     else
  // 	m_Position.z += m_speed;
  //   }

  // else if (event.type == sf::Event::MouseMoved)
  //   {
  //     m_theta -= (m_App->GetInput().GetMouseX() - Last_Mouse.x) * m_sensivity;
  //     m_phi   -= (m_App->GetInput().GetMouseY() - Last_Mouse.y) * m_sensivity;
  //     Last_Mouse.x = m_App->GetInput().GetMouseX();
  //     Last_Mouse.y = m_App->GetInput().GetMouseY();
  //     VectorsFromAngles();
  //   }

  else if (event.type == sf::Event::Resized)
    {
      sf::Vector2u	WSize = m_RWindow.getSize();
      sf::Vector2f	WSizef(WSize.x, WSize.y);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      // gluPerspective(70, static_cast<double>(this->m_RWindow.getSize().x) / this->m_RWindow.getSize().y, 1.0,10000.0);
      // glViewport(0,0, this->m_RWindow.getSize().x, this->m_RWindow.getSize().y);

      gluPerspective(70.0, WSizef.x / WSizef.y, 1.0,10000.0);
      // gluPerspective(70.0, 1.0f, 1.0,10000.0);

      // gluPerspective(70.0, WSizef.x / WSizef.y, 1.0,10000.0);

      glViewport(0,0, WSize.x, WSize.y);


      glMatrixMode(GL_MODELVIEW);
    }
}

void	Camera_FreeFly::Update(const float& elapsedTime)
{
  static_cast<void>(elapsedTime);

  if (m_Active == true)
    {


      sf::Vector2u	WSize = m_RWindow.getSize();
      sf::Vector2f	HWSizef(WSize.x / 2, WSize.y / 2);
      sf::Vector2i	HWSizei(WSize.x / 2, WSize.y / 2);

      sf::Vector2f	MousePos(sf::Mouse::getPosition(m_RWindow).x, sf::Mouse::getPosition(m_RWindow).y);


      m_theta -= (MousePos.x - HWSizef.x) * m_sensivity;
      m_phi   -= (MousePos.y - HWSizef.y) * m_sensivity;
      VectorsFromAngles();


      // m_RWindow.SetCursorPosition(m_RWindow.getSize().x / 2, m_RWindow.getSize().y / 2);
      sf::Mouse::setPosition(HWSizei, m_RWindow);


      bool	speed_up = sf::Keyboard::isKeyPressed( sf::Keyboard::X );

      if (speed_up)
	m_speed *= 20;

      if (sf::Keyboard::isKeyPressed( sf::Keyboard::A ))
	m_Position -= m_Left * m_speed;
      else if (sf::Keyboard::isKeyPressed( sf::Keyboard::D ))
	m_Position += m_Left * m_speed;

      if (sf::Keyboard::isKeyPressed( sf::Keyboard::W ))
	m_Position +=  m_Forward * m_speed;
      else if (sf::Keyboard::isKeyPressed( sf::Keyboard::S ))
	m_Position -=  m_Forward * m_speed;

      if (speed_up)
	m_speed /= 20;

      m_Target = m_Position + m_Forward;
    }
}

void	Camera_FreeFly::Look()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(m_Position.x, m_Position.y, m_Position.z,
	    m_Target.x, m_Target.y, m_Target.z,
	    0,0,1);
}

void	Camera_FreeFly::SetTarget(sf::Vector3f Target)
{
  m_Target = Target;
}

const float&	Camera_FreeFly::GetTheta() const
{
  return (m_theta);
}

const float&	Camera_FreeFly::GetPhi() const
{
  return (m_phi);
}

const	sf::Vector3f&	Camera_FreeFly::GetPosition() const
{
  return (m_Position);
}

void		Camera_FreeFly::SetActive(bool Active)
{
  m_Active = Active;
}

bool		Camera_FreeFly::GetActive() const
{
  return (m_Active);
}
