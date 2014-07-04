

#ifndef	MULTIDRAW_HPP
#define	MULTIDRAW_HPP


#include <SFML/Graphics.hpp>


class	MultiDraw
{
private :

  static std::vector<sf::Vertex> *	m_pVertices;

  static sf::Font*	m_pFont;
  static sf::Text*	m_pText;

  static int		m_minimal;

  static sf::View*	m_pCamera;
  static sf::Vector2f	m_min, m_max;

public :

  static void	Create();
  static void	Destroy();

public :

  static void	Minimal(int enable);

public :

  static void	Clear();

  static void	AddVertex(float x, float y, const sf::Color& color);
  static void	AddVertex(const sf::Vector2f& pos, const sf::Color& color);
  static void	AddVertex(const sf::Vertex& newVertex);

  static void	AddLine(sf::PrimitiveType PType,
			const sf::Vector2f& pos1, const sf::Vector2f& pos2,
			float thickness, const sf::Color& color);

  static void	AddLine(sf::PrimitiveType PType,
			float x1, float y1, float x2, float y2,
			float thickness, const sf::Color& color)
  {
    MultiDraw::AddLine(PType, sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), thickness, color);
  }

  static void	AddLine(sf::PrimitiveType PType,
			const sf::Vector2f& pos1, const sf::Vector2f& pos2,
			float thickness1, float thickness2, const sf::Color& color);

  static void	AddLine(sf::PrimitiveType PType,
			float x1, float y1, float x2, float y2,
			float thickness1, float thickness2, const sf::Color& color)
  {
    MultiDraw::AddLine(PType, sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), thickness1, thickness2, color);
  }

  ///

  static void	AddCircle(sf::PrimitiveType PType,
			  const sf::Vector2f& pos, float radius, const sf::Color& color);

  static void	AddCircle(sf::PrimitiveType PType,
			  float x, float y, float radius, const sf::Color& color)
  {
    MultiDraw::AddCircle(PType, sf::Vector2f(x, y), radius, color);
  }

  ///

  static void	AddSquare(sf::PrimitiveType PType,
			  const sf::Vector2f& min, const sf::Vector2f& max,
			  const sf::Color& color);

  static void	AddSquare(sf::PrimitiveType PType,
			  float min_x, float min_y, float max_x, float max_y,
			  const sf::Color& color)
  {
    AddSquare(PType, sf::Vector2f(min_x, min_y), sf::Vector2f(max_x, max_y), color);
  }

  ///

  static void	Render(sf::RenderWindow& RWindow, sf::PrimitiveType PType);

  static void	PrintNow(sf::RenderWindow& RWindow,
  			 const std::string& text,
  			 const sf::Vector2f& pos,
  			 int charsize = 30,
  			 const sf::Color& color = sf::Color::Black,
  			 bool mask = false);

};


#endif

