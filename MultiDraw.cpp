

#include "MultiDraw.hpp"

#include "Utils.hpp"

#include <iostream>



std::vector<sf::Vertex> *	MultiDraw::m_pVertices = NULL;

sf::Font*	MultiDraw::m_pFont = NULL;
sf::Text*	MultiDraw::m_pText = NULL;

int		MultiDraw::m_minimal = 1;


///////////////////////////////////////////

void	MultiDraw::Create()
{

  if (m_pVertices == NULL)
    {
      m_pVertices = new std::vector<sf::Vertex>;
      m_pVertices->reserve(1000);
    }

  if (m_pFont == NULL)
    {
      m_pFont = new sf::Font;

      if (m_pFont->loadFromFile("res/arial.ttf") )
  	{
  	  m_pText = new sf::Text;
  	  m_pText->setFont( *m_pFont );
  	}
      else
  	delete m_pFont, m_pFont = NULL;
    }
}

void	MultiDraw::Destroy()
{
  delete m_pVertices, m_pVertices = NULL;

  delete m_pFont, m_pFont = NULL;
  delete m_pText, m_pText = NULL;
}

///////////////////////////////////////////

void	MultiDraw::Minimal(int enable)
{
  if (enable <= 1)
    m_minimal = 1;
  else if (enable >= 3)
    m_minimal = 4;
  else
    m_minimal = 2;
}

///////////////////////////////////////////

void	MultiDraw::Clear()
{
  if (m_pVertices != NULL)
    m_pVertices->clear();
}

///

void	MultiDraw::AddVertex(float x, float y, const sf::Color& color)
{
  if (m_pVertices != NULL)
    AddVertex( sf::Vector2f( x, y ), color );
}

void	MultiDraw::AddVertex(const sf::Vector2f& pos, const sf::Color& color)
{
  if (m_pVertices != NULL)
    AddVertex( sf::Vertex( pos, color ) );
}

void	MultiDraw::AddVertex(const sf::Vertex& newVertex)
{
  if (m_pVertices != NULL)
    {
      if (m_pVertices->size() >= m_pVertices->capacity())
	m_pVertices->reserve( m_pVertices->capacity() * 2 );

      m_pVertices->push_back( newVertex );
    }
}

///

void	MultiDraw::AddLine(sf::PrimitiveType PType,
			   const sf::Vector2f& pos1, const sf::Vector2f& pos2,
			   float thickness, const sf::Color& color)
{

  static sf::Vector2f	normal;
  static float		len;

  if (m_pVertices != NULL)
    {

      if (PType == sf::Points || PType == sf::Lines)
	{
	  AddVertex( sf::Vertex( pos1, color) );
	  AddVertex( sf::Vertex( pos2, color) );
	}

      else if (PType == sf::Triangles)
	{
	  // sf::Vector2f	normal(pos1.y - pos2.y, pos2.x - pos1.x);

	  normal.x = pos1.y - pos2.y;
	  normal.y = pos2.x - pos1.x;

	  // float		len = length<float>(normal.x, normal.y);
	  len = length(normal.x, normal.y);

	  if (len == 0.0f)
	    return;

	  normal /= len;
	  normal *= thickness / 2;


	  AddVertex( sf::Vertex( pos1 - normal , color) );
	  AddVertex( sf::Vertex( pos2 - normal , color) );
	  AddVertex( sf::Vertex( pos2 + normal , color) );

	  AddVertex( sf::Vertex( pos1 - normal , color) );
	  AddVertex( sf::Vertex( pos2 + normal , color) );
	  AddVertex( sf::Vertex( pos1 + normal , color) );
	}

      else if (PType == sf::TrianglesFan || PType == sf::Quads)
	{
	  // sf::Vector2f	normal(pos1.y - pos2.y, pos2.x - pos1.x);

	  normal.x = pos1.y - pos2.y;
	  normal.y = pos2.x - pos1.x;

	  // float		len = length<float>(normal.x, normal.y);
	  len = length(normal.x, normal.y);

	  if (len == 0.0f)
	    return;

	  normal /= len;
	  normal *= thickness;


	  AddVertex( sf::Vertex( pos1 - normal , color) );
	  AddVertex( sf::Vertex( pos2 - normal , color) );
	  AddVertex( sf::Vertex( pos2 + normal , color) );
	  AddVertex( sf::Vertex( pos1 + normal , color) );
	}

      else
	{
	  std::cerr << __FILE__ << " / " << __func__ << std::endl;
	  std::cerr << "Points / Lines / TrianglesFan / Quads" << std::endl;
	  exit(1);
	}

    }
}

void	MultiDraw::AddLine(sf::PrimitiveType PType,
			   const sf::Vector2f& pos1, const sf::Vector2f& pos2,
			   float thickness1, float thickness2, const sf::Color& color)
{

  static sf::Vector2f	normal1, normal2;
  static float		len;

  if (m_pVertices != NULL)
    {

      if (PType == sf::Points || PType == sf::Lines)
	{
	  AddVertex( sf::Vertex( pos1, color) );
	  AddVertex( sf::Vertex( pos2, color) );
	}

      else if (PType == sf::Triangles)
	{
	  // sf::Vector2f	normal(pos1.y - pos2.y, pos2.x - pos1.x);

	  normal1.x = pos1.y - pos2.y;
	  normal1.y = pos2.x - pos1.x;

	  // float		len = length(normal.x, normal.y);
	  len = length(normal1.x, normal1.y);

	  if (len == 0.0f)
	    return;

	  normal1 /= len;

	  normal2 = normal1;
	  normal2 *= thickness2 / 2;
	  normal1 *= thickness1 / 2;


	  AddVertex( sf::Vertex( pos1 - normal1 , color) );
	  AddVertex( sf::Vertex( pos2 - normal2 , color) );
	  AddVertex( sf::Vertex( pos2 + normal2 , color) );

	  AddVertex( sf::Vertex( pos1 - normal1 , color) );
	  AddVertex( sf::Vertex( pos2 + normal2 , color) );
	  AddVertex( sf::Vertex( pos1 + normal1 , color) );
	}

      else if (PType == sf::TrianglesFan || PType == sf::Quads)
	{
	  // sf::Vector2f	normal(pos1.y - pos2.y, pos2.x - pos1.x);

	  normal1.x = pos1.y - pos2.y;
	  normal1.y = pos2.x - pos1.x;

	  // float		len = length(normal.x, normal.y);
	  len = length(normal1.x, normal1.y);

	  if (len == 0.0f)
	    return;

	  normal1 /= len;

	  normal2 = normal1;
	  normal2 *= thickness2 / 2;
	  normal1 *= thickness1 / 2;

	  // normal.x = pos1.y - pos2.y;
	  // normal.y = pos2.x - pos1.x;

	  // // float		len = length(normal.x, normal.y);
	  // len = length(normal.x, normal.y);

	  // if (len == 0.0f)
	  //   return;

	  // normal /= len;
	  // normal *= thickness;


	  AddVertex( sf::Vertex( pos1 - normal1 , color) );
	  AddVertex( sf::Vertex( pos2 - normal2 , color) );
	  AddVertex( sf::Vertex( pos2 + normal2 , color) );
	  AddVertex( sf::Vertex( pos1 + normal1 , color) );
	}

      else
	{
	  std::cerr << __FILE__ << " / " << __func__ << std::endl;
	  std::cerr << "Points / Lines / TrianglesFan / Quads" << std::endl;
	  exit(1);
	}

    }
}

void	MultiDraw::AddCircle(sf::PrimitiveType PType,
			     const sf::Vector2f& pos, float radius, const sf::Color& color)
{
  if (m_pVertices != NULL)
    {

      ///

      static sf::Vector2f	coord;
      static float*		angles = NULL;

#define	D_CIRCLE_QUALITY	16
#define	D_ANGLE_DATA		(D_CIRCLE_QUALITY + 1)

      if (angles == NULL)
	{
	  angles = new float[ D_ANGLE_DATA * 2 ];

	  for (int i = 0; i < D_ANGLE_DATA; ++i)
	    {
	      angles[i] = cosf( (M_PI * 2 / D_CIRCLE_QUALITY) * i );
	      angles[i + D_ANGLE_DATA] = sinf( (M_PI * 2 / D_CIRCLE_QUALITY) * i );
	    }
	}

      ///

      if (PType == sf::Points)
	{
	  for (int i = 0; i < D_CIRCLE_QUALITY; i += m_minimal)
	    {
	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );
	    }
	}

      else if (PType == sf::Lines)
	{

	  for (int i = 0; i < D_CIRCLE_QUALITY;)
	    {
	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );

	      // ++i;
	      i += m_minimal;

	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );
	    }
	}

      else if (PType == sf::Triangles)
	{
	  for (int i = 0; i < D_CIRCLE_QUALITY;)
	    {
	      AddVertex( sf::Vertex( pos , color) );

	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );

	      // ++i;
	      i += m_minimal;

	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );
	    }
	}

      else if (PType == sf::TrianglesFan)
	{
	  AddVertex( sf::Vertex( pos , color) );

	  // for (int i = 0; i < D_CIRCLE_QUALITY; ++i)
	  for (int i = 0; i < D_CIRCLE_QUALITY; i += (m_minimal ? 2 : 1))
	    {
	      // coord.x = pos.x + radius * cosf( (M_PI / 8) * i );
	      // coord.y = pos.y + radius * sinf( (M_PI / 8) * i );

	      coord.x = pos.x + radius * angles[ i ];
	      coord.y = pos.y + radius * angles[ i + D_ANGLE_DATA ];

	      AddVertex( sf::Vertex( coord, color) );
	    }
	}

      else
	{
	  std::cerr << __FILE__ << " / " << __func__ << std::endl;
	  std::cerr << "Points / Lines / Triangles / TrianglesFan" << std::endl;
	  exit(1);
	}

    }
}


void	MultiDraw::AddSquare(sf::PrimitiveType PType,
			     const sf::Vector2f& min, const sf::Vector2f& max,
			     const sf::Color& color)
{
  if (m_pVertices != NULL)
    {

      static sf::Vector2f	coord1;
      static sf::Vector2f	coord2;

      coord1.x = min.x;
      coord1.y = max.y;

      coord2.x = max.x;
      coord2.y = min.y;

      if (PType == sf::Points)
	{
	  AddVertex( sf::Vertex( coord1, color) );
	  AddVertex( sf::Vertex( coord2, color) );
	  AddVertex( sf::Vertex( min, color) );
	  AddVertex( sf::Vertex( max, color) );
	}

      else if (PType == sf::Lines)
	{
	  AddVertex( sf::Vertex( min, color) );
	  AddVertex( sf::Vertex( coord2, color) );

	  AddVertex( sf::Vertex( coord2, color) );
	  AddVertex( sf::Vertex( max, color) );

	  AddVertex( sf::Vertex( max, color) );
	  AddVertex( sf::Vertex( coord1, color) );

	  AddVertex( sf::Vertex( coord1, color) );
	  AddVertex( sf::Vertex( min, color) );
	}

      else if (PType == sf::Triangles)
	{
	  AddVertex( sf::Vertex( min, color) );
	  AddVertex( sf::Vertex( coord2, color) );
	  AddVertex( sf::Vertex( max, color) );

	  AddVertex( sf::Vertex( min, color) );
	  AddVertex( sf::Vertex( coord1, color) );
	  AddVertex( sf::Vertex( max, color) );
	}

      else if (PType == sf::TrianglesFan || PType == sf::Quads)
	{
	  AddVertex( sf::Vertex( min, color) );
	  AddVertex( sf::Vertex( coord2, color) );
	  AddVertex( sf::Vertex( max, color) );
	  AddVertex( sf::Vertex( coord1, color) );
	}

      else
	{
	  std::cerr << __FILE__ << " / " << __func__ << std::endl;
	  std::cerr << "Points / Lines / Triangles / TrianglesFan / Quads" << std::endl;
	  exit(1);
	}
    }
}


void	MultiDraw::Render(sf::RenderWindow& RWindow, sf::PrimitiveType PType)
{
  if (m_pVertices != NULL && !m_pVertices->empty())
    RWindow.draw(&(m_pVertices->front()), m_pVertices->size(), PType);
}


void	MultiDraw::PrintNow(sf::RenderWindow& RWindow,
			    const std::string& text,
			    const sf::Vector2f& pos,
			    int charsize /*= 30*/,
			    const sf::Color& color /*= sf::Black*/,
			    bool mask /*= false*/)
{
  if (m_pText != NULL)
    {
      m_pText->setString( sf::String( text ) );
      m_pText->setPosition( pos );
      m_pText->setCharacterSize( charsize );
      m_pText->setColor( color );

      if (mask)
	{
	  sf::FloatRect	tmpFRect = m_pText->getGlobalBounds();

	  sf::RectangleShape	Mask;
	  Mask.setPosition( tmpFRect.left, tmpFRect.top );
	  Mask.setSize( sf::Vector2f(tmpFRect.width, tmpFRect.height) );
	  Mask.setFillColor( sf::Color(0,0,0, 128) );

	  RWindow.draw( Mask );
	}

      RWindow.draw( *m_pText );
    }
}


#undef	D_IS_VISIBLE

