

#ifndef	MY_GAME_HH
#define	MY_GAME_HH

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include <list>


#include "3D/RessourceManager.hh"

# include "3D/Camera_FreeFly.hh"


# define FRAMERATE 30.0f
# define COTECASE 10.0f

class	SubAlien;
class	SubPlayer;

class	Game
{
private:

  Camera_FreeFly	m_camera;
  sf::RenderWindow	m_RWindow;

  std::list<std::string>	m_all_Name;
  std::list<std::string>	m_all_Name2;

  std::list<SubAlien>	m_SubAliens;
  std::list<SubPlayer>	m_SubPlayers;


  bool	line_mode;
  int	m_flags;


  bool	aaliasing;


public:

  Game();
  ~Game();

  void	Run()
  {
    sf::Clock	Clock;

    while (this->m_RWindow.isOpen())
      {
	float time = Clock.restart().asSeconds();

        this->Update(time);

	if (this->m_RWindow.isOpen())
	  {
	    this->Render();
	    this->m_RWindow.display();
	  }
      }
  }

  void	Update(const float elapsedTime);
  void	Render(void);

};

#endif

