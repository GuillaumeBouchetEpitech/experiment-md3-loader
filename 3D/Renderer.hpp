

#ifndef	RENDERER_HPP
#define	RENDERER_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

#include "Texture1D.hh"


class	Renderer
{
public :

  static sf::Texture*			m_pTexture;

  static std::vector<sf::Vector3f>	m_Vertices;
  static std::vector<sf::Vector3f>	m_Normales;
  static std::vector<sf::Vector2f>	m_TexCoords;
  static std::vector<sf::Vector3f>	m_Colors;
  static std::vector<unsigned int>	m_Indices;
  // static std::vector<unsigned int>	m_Indices2;

  static std::vector<sf::Vector3f>	m_ShadowVertices;

  static Texture1D			m_Tex1D;
  static std::vector<float>		m_TexCoords1D;

  static std::vector<sf::Vector3f>	m_sec_Vertices;
  static std::vector<sf::Vector3f>	m_sec_Normales;
  static std::vector<sf::Vector2f>	m_sec_TexCoords;
  static std::vector<sf::Vector3f>	m_sec_Colors;
  static std::vector<float>		m_sec_TexCoords1D;

public :

  static void	Initialize();

public :

  enum
  {
    // eColor,
    eTexture			= 1 << 0,
    eNormal			= 1 << 1,
    eCellShading_texture1D	= 1 << 2,
    eCellShading_color		= 1 << 3,
    eCellShading_bound		= 1 << 4
  };

public :

  static void	ProtoRender(int flags = 0);

  static void	CastShadow(const sf::Vector3f& lightPosition);

};


#endif

