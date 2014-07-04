

#ifndef	RESSOURCEMANAGER_HH
#define	RESSOURCEMANAGER_HH


#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


class   AAnimation;
class   AnimationPlayer;
class   AnimationAlien;


class	MyRessourceBase
{
public :
  MyRessourceBase();
  virtual ~MyRessourceBase();

};



template<typename T>
class	MyRessource : public MyRessourceBase
{
public :
  MyRessource(T* Data);
  virtual ~MyRessource();

  T*	GetData() const;

private :
  T*	m_Data;

};



class   RessourceManager
{
private :

  RessourceManager();

  ~RessourceManager();

  static RessourceManager*	m_Instance;

public :

  static RessourceManager*	GetInstance()
  {
    if (m_Instance == NULL)
      m_Instance = new RessourceManager;
    return (m_Instance);
  }

  static void	Kill()
  {
    if (m_Instance != NULL)
      {
	delete m_Instance;
	m_Instance = NULL;
      }
  }

public :

  void	Infos();

public :

  enum e_RscManagerLOD
    {
      LodLow,
      LodMedium,
      LodHigh,

      LodDefault = LodHigh,
    };

  void	SetLOD(e_RscManagerLOD newLOD);

public :

  sf::Texture*	LoadTexture(const std::string& Filename, bool root_directory = true);
  AnimationPlayer*	LoadPlayer(const std::string& PathName);
  AnimationAlien*	LoadAlien(const std::string& PathName);

  sf::Texture*	GetTexture(const std::string& Filename);
  AnimationPlayer*	GetPlayer(const std::string& Filename);
  AnimationAlien*	GetAlien(const std::string& PathName);

private :

  std::map< std::string, MyRessourceBase* >	m_Animations;
  std::map< std::string, MyRessourceBase* >	m_Images;

  e_RscManagerLOD				m_currentLOD;

};

#endif
