

#include <map>
#include <string>

#include "RessourceManager.hh"
#include "RuntimeException.hh"

#include "Md3/Md3LodPlayer.hh"
#include "Md3/Md3Alien.hh"


#include "AAnimation.hh"


static std::string	resource_root(FILE_PATH);


MyRessourceBase::MyRessourceBase()
{
}

MyRessourceBase::~MyRessourceBase()
{
}




template<typename T>
MyRessource<T>::MyRessource(T* Data)
{
  m_Data = Data;
}

template<typename T>
MyRessource<T>::~MyRessource()
{
  delete m_Data;
}

template<typename T>
T*	MyRessource<T>::GetData() const
{
  return (m_Data);
}



RessourceManager*	RessourceManager::m_Instance = NULL;

RessourceManager::RessourceManager()
{
  m_currentLOD = LodDefault;
}

RessourceManager::~RessourceManager()
{
  for (typename std::map< std::string, MyRessourceBase* >::iterator itr = m_Images.begin();
       itr != m_Images.end();
       ++itr)
    delete itr->second;
  for (typename std::map< std::string, MyRessourceBase* >::iterator itr = m_Animations.begin();
       itr != m_Animations.end();
       ++itr)
    delete itr->second;
}

void	RessourceManager::Infos()
{
  std::cout << resource_root << std::endl;

  for (typename std::map< std::string, MyRessourceBase* >::iterator itr = m_Images.begin();
       itr != m_Images.end();
       ++itr)
    std::cout << itr->first << std::endl;

  for (typename std::map< std::string, MyRessourceBase* >::iterator itr = m_Animations.begin();
       itr != m_Animations.end();
       ++itr)
    std::cout << itr->first << std::endl;
}

void	RessourceManager::SetLOD(e_RscManagerLOD newLOD)
{
  if (m_currentLOD != newLOD)
    {
      m_currentLOD = newLOD;
      for (typename std::map< std::string, MyRessourceBase* >::iterator itr = m_Animations.begin();
	   itr != m_Animations.end();
	   ++itr)
	static_cast< MyRessource< AAnimation > * >(itr->second)->GetData()->SetLOD(newLOD);
    }
}

sf::Texture*	RessourceManager::LoadTexture(const std::string& Filename, bool root_directory)
{
  if (m_Images.find(Filename) == m_Images.end())
    {
      sf::Texture*	Ressource = new sf::Texture;

      if (Ressource->loadFromFile( (root_directory ? resource_root : "") + Filename ) == false)
	{
	  delete Ressource;
	  throw RuntimeException("Load fail: bad filename", "Load Image");
	}

      Ressource->setSmooth(true);

      MyRessource< sf::Texture > *	mRessource = new MyRessource< sf::Texture >(Ressource);
      m_Images[ Filename ] = mRessource;
      return (Ressource);
    }
  return (GetTexture(Filename));
}

AnimationPlayer*	RessourceManager::LoadPlayer(const std::string& PathName)
{
  if (m_Animations.find(PathName) == m_Animations.end())
    {
      AnimationPlayer*	Ressource = new AnimationPlayer(resource_root + PathName);

      if (Ressource != NULL)
	{
	  MyRessource< AnimationPlayer >*	mRessource;
	  mRessource = new MyRessource< AnimationPlayer >(Ressource);
	  m_Animations[ PathName ] = mRessource;
	  return (Ressource);
	}

      else
	{
	  delete Ressource;
	  throw RuntimeException("Load fail: bad filename", "Load Player");
	  return (NULL);
	}
    }
  return (GetPlayer(PathName));
}

AnimationAlien*	RessourceManager::LoadAlien(const std::string& PathName)
{
  if (m_Animations.find(PathName) == m_Animations.end())
    {
      AnimationAlien*	Ressource = new AnimationAlien(resource_root + PathName);

      if (Ressource != NULL)
	{
	  MyRessource< AnimationAlien >*	mRessource;
	  mRessource = new MyRessource< AnimationAlien >(Ressource);
	  m_Animations[ PathName ] = mRessource;
	  return (Ressource);
	}

      else
	{
	  delete Ressource;
	  throw RuntimeException("Load fail: bad filename", "Load Alien");
	  return (NULL);
	}
    }
  return (GetAlien(PathName));
}


sf::Texture*	RessourceManager::GetTexture(const std::string& Filename)
{
  std::map< std::string, MyRessourceBase* >::iterator itr;

  itr = m_Images.find(Filename);

  if (itr == m_Images.end())
    return (NULL);

  return ((static_cast< MyRessource< sf::Texture > * >(itr->second))->GetData());
}

AnimationPlayer*	RessourceManager::GetPlayer(const std::string& Filename)
{
  std::map< std::string, MyRessourceBase* >::iterator itr;
  itr = m_Animations.find(Filename);
  if (itr == m_Animations.end())
    return (NULL);
  return ((static_cast< MyRessource< AnimationPlayer > * >(itr->second))->GetData());
}

AnimationAlien*	RessourceManager::GetAlien(const std::string& Filename)
{
  std::map< std::string, MyRessourceBase* >::iterator itr;

  itr = m_Animations.find(Filename);

  if (itr == m_Animations.end())
    return (NULL);

  return ((static_cast< MyRessource< AnimationAlien > * >(itr->second))->GetData());
}


