

#ifndef	CAMERA_FREEFLY_HH
#define	CAMERA_FREEFLY_HH


// #include "Math/Vector2.hpp"
// #include "Math/Vector3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

class Camera_FreeFly
{
public:
  Camera_FreeFly(sf::RenderWindow& RWindow);

  void  VectorsFromAngles();

  void	Initialize();

  void	Event(const sf::Event& Event);

  void	Update(const float& elapsedTime);

  void	Look();


  void	SetTarget(sf::Vector3f Target);

  const float	&GetTheta() const;
  const float	&GetPhi() const;
  const	sf::Vector3f&	GetPosition() const;

  void		SetActive(bool Active);
  bool		GetActive() const;


private:
  sf::RenderWindow&	m_RWindow;

  sf::Vector3f	m_Position;
  sf::Vector3f	m_Forward;
  sf::Vector3f	m_Up;
  sf::Vector3f	m_Left;
  sf::Vector3f	m_Target;
  float		m_speed;
  float		m_sensivity;
  float		m_theta;
  float		m_phi;

  bool		m_Active;

};

#endif
