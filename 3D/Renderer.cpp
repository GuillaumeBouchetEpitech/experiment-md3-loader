

#include "Renderer.hpp"

#include <cmath>


sf::Texture*			Renderer::m_pTexture = NULL;

std::vector<sf::Vector3f>	Renderer::m_Vertices;
std::vector<sf::Vector3f>	Renderer::m_Normales;
std::vector<sf::Vector2f>	Renderer::m_TexCoords;
std::vector<sf::Vector3f>	Renderer::m_Colors;
std::vector<unsigned int>	Renderer::m_Indices;

std::vector<sf::Vector3f>	Renderer::m_ShadowVertices;
// std::vector<unsigned int>	Renderer::m_Indices2;

Texture1D			Renderer::m_Tex1D;
std::vector<float>		Renderer::m_TexCoords1D;


std::vector<sf::Vector3f>	Renderer::m_sec_Vertices;
std::vector<sf::Vector3f>	Renderer::m_sec_Normales;
std::vector<sf::Vector2f>	Renderer::m_sec_TexCoords;
std::vector<sf::Vector3f>	Renderer::m_sec_Colors;
std::vector<float>		Renderer::m_sec_TexCoords1D;




void	Renderer::Initialize()
{
  m_Vertices.reserve(2048);
  m_Normales.reserve(2048);
  m_TexCoords.reserve(2048);
  m_Indices.reserve(2048);

  // m_Indices2.reserve(2048);
  m_ShadowVertices.reserve(2048 * 4);

  m_TexCoords1D.reserve(2048);

  m_sec_Vertices.reserve(2048);
  m_sec_Normales.reserve(2048);
  m_sec_TexCoords.reserve(2048);
  m_sec_Colors.reserve(2048);
  m_sec_TexCoords1D.reserve(2048);
}




void	RotateVector( float* mat, sf::Vector3f& V, sf::Vector3f& D )
{
  D.x = (mat[0] * V.x) + (mat[4] * V.y) + (mat[8]  * V.z);
  D.y = (mat[1] * V.x) + (mat[5] * V.y) + (mat[9]  * V.z);
  D.z = (mat[2] * V.x) + (mat[6] * V.y) + (mat[10] * V.z);
}

float	Magnitude( sf::Vector3f& V )
{
  return ( sqrtf( (V.x * V.x) + (V.y * V.y) + (V.z * V.z) ) );
}

void	Normalize( sf::Vector3f& V )
{
  float M = Magnitude (V);

  if (M != 0.0f)
    {
      V.x /= M;
      V.y /= M;
      V.z /= M;
    }
}

float	DotProduct( sf::Vector3f& V1, sf::Vector3f& V2 )
{
  return ( (V1.x * V2.x) + (V1.y * V2.y) + (V1.z * V2.z) );
}


    sf::Vector3f*	cur_vert[3];

// bool	IsVisible( const sf::Vector3f& V1,
// 		   const sf::Vector3f& V2,
// 		   const sf::Vector3f& V3,
// 		   const sf::Vector3f& VX )
bool	IsVisible( sf::Vector3f** V3, const sf::Vector3f& VX )
{
  float	a, b, c, d;

  a = V3[0]->y * (V3[1]->z - V3[2]->z) + V3[1]->y * (V3[2]->z - V3[0]->z) + V3[2]->y * (V3[0]->z - V3[1]->z);
  b = V3[0]->z * (V3[1]->x - V3[2]->x) + V3[1]->z * (V3[2]->x - V3[0]->x) + V3[2]->z * (V3[0]->x - V3[1]->x);
  c = V3[0]->x * (V3[1]->y - V3[2]->y) + V3[1]->x * (V3[2]->y - V3[0]->y) + V3[2]->x * (V3[0]->y - V3[1]->y);
  d = -( V3[0]->x * (V3[1]->y * V3[2]->z - V3[2]->y * V3[1]->z) +
	 V3[1]->x * (V3[2]->y * V3[0]->z - V3[0]->y * V3[2]->z) +
	 V3[2]->x * (V3[0]->y * V3[1]->z - V3[1]->y * V3[0]->z) );

  return ( (a * VX.x + b * VX.y + c * VX.z + d * 1.0f) > 0 );
}


void	Renderer::ProtoRender(int flags)
{


  ///

  // std::vector<unsigned int>	tmp_Indices;

  // for (unsigned int i = 0; i < m_Indices.size(); i += 6)
  //   {
  //     tmp_Indices.push_back( m_Indices[ i + 0 ] );
  //     tmp_Indices.push_back( m_Indices[ i + 1 ] );
  //     tmp_Indices.push_back( m_Indices[ i + 2 ] );
  //   }

  // m_Indices = tmp_Indices;

  ///

  if ( flags & eCellShading_texture1D || flags & eCellShading_color )
    {
      sf::Vector3f	lightAngle(0,0,1);
      // Normalize( lightAngle );

      float	tmp_matrix[16];
      glGetFloatv( GL_MODELVIEW_MATRIX, tmp_matrix );


      m_sec_Vertices.clear();
      m_sec_Normales.clear();
      m_sec_TexCoords.clear();
      m_sec_Colors.clear();
      m_sec_TexCoords1D.clear();

      for (unsigned int i = 0; i < m_Indices.size(); ++i)
      // for (unsigned int i = 0; i < m_Indices2.size(); ++i)
	{

#define	D_IDX	m_Indices[i]
// #define	D_IDX	m_Indices2[i]

	  m_sec_Vertices.push_back( m_Vertices[ D_IDX ] );

	  if ( flags & eNormal )
	    m_sec_Normales.push_back( m_Normales[ D_IDX ] );

	  if ( flags & eTexture )
	    m_sec_TexCoords.push_back( m_TexCoords[ D_IDX ] );


	  {
	    sf::Vector3f	tmp_vector;

	    RotateVector( tmp_matrix, m_Normales[ D_IDX ], tmp_vector );
	    Normalize( tmp_vector );

	    float	tmp_shade = DotProduct( tmp_vector, lightAngle );

	    if (tmp_shade < 0.0f)
	      tmp_shade = 0.0f;

	    if ( flags & eCellShading_texture1D )
	      m_sec_TexCoords1D.push_back( tmp_shade );

	    if ( flags & eCellShading_color )
	      m_sec_Colors.push_back( sf::Vector3f(tmp_shade, tmp_shade, tmp_shade) );

	  }

#undef	D_IDX

	} // for (unsigned int i = 0; i < m_Indices.size(); ++i)

    }






  {

    {
      glEnableClientState( GL_VERTEX_ARRAY );

      if ( flags & eCellShading_texture1D || flags & eCellShading_color )
	glVertexPointer( 3, GL_FLOAT, 0, &(m_sec_Vertices[0].x) );
      else
	glVertexPointer( 3, GL_FLOAT, 0, &(m_Vertices[0].x) );
    }


    if ( flags & eNormal )
      {
	glEnableClientState( GL_NORMAL_ARRAY );


	glEnable(GL_LIGHTING);


	if ( flags & eCellShading_texture1D || flags & eCellShading_color )
	  glNormalPointer( GL_FLOAT, 0, &(m_sec_Normales[0].x) );
	else
	  glNormalPointer( GL_FLOAT, 0, &(m_Normales[0].x) );
      }
    else
      glDisable(GL_LIGHTING);


    if ( flags & eCellShading_color )
      {
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 3, GL_FLOAT, 0, &(m_sec_Colors[0].x) );
      }


    if ( flags & eCellShading_texture1D )
      {
	glClientActiveTexture(GL_TEXTURE1);
	glActiveTextureARB(GL_TEXTURE1_ARB);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_1D);
	m_Tex1D.Bind();

	glTexCoordPointer( 1, GL_FLOAT, 0, &(m_sec_TexCoords1D[0]) );
      }


    if ( flags & eTexture )
      {
	glClientActiveTexture(GL_TEXTURE0);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind( m_pTexture );
	m_pTexture->setSmooth(false);

	if ( flags & eCellShading_texture1D || flags & eCellShading_color )
	  glTexCoordPointer( 2, GL_FLOAT, 0, &(m_sec_TexCoords[0].x) );
	else
	  glTexCoordPointer( 2, GL_FLOAT, 0, &(m_TexCoords[0].x) );
      }


    if ( flags & eCellShading_texture1D || flags & eCellShading_color )
      glDrawArrays( GL_TRIANGLES, 0, m_sec_Vertices.size() );
    else
      glDrawElements( GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, &(m_Indices[0]) );
      // glDrawElements( GL_TRIANGLES, m_Indices2.size(), GL_UNSIGNED_INT, &(m_Indices2[0]) );

    {
      glDisableClientState( GL_VERTEX_ARRAY );

      if ( flags & eNormal )
	glDisableClientState( GL_NORMAL_ARRAY );

      if ( flags & eCellShading_color )
	glDisableClientState( GL_COLOR_ARRAY );

      if ( flags & eCellShading_texture1D )
	{
	  glClientActiveTexture(GL_TEXTURE1);
	  glActiveTexture(GL_TEXTURE1);
	  glDisable(GL_TEXTURE_1D);
	  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}


      glClientActiveTexture( GL_TEXTURE0 );
      glActiveTexture(GL_TEXTURE0);

      if ( flags & eTexture )
	{
	  glDisable(GL_TEXTURE_2D);
	  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
    }

  }


  if ( flags & eCellShading_bound )
    {

      {
	glDisable(GL_LIGHTING);

	glEnable(GL_CULL_FACE);

	glColor3ub(0,0,0);

	{
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  // glCullFace(GL_BACK);
	  glCullFace(GL_FRONT);
	}

	glDepthFunc (GL_LEQUAL);


	glLineWidth(3.0f);
      }


      glEnableClientState( GL_VERTEX_ARRAY );
      glVertexPointer( 3, GL_FLOAT, 0, &(m_Vertices[0].x) );

      glDrawElements( GL_TRIANGLES, m_Indices.size(),
      		      GL_UNSIGNED_INT, &(m_Indices[0]) );
      // glDrawElements( GL_TRIANGLES, m_Indices2.size(),
      // 		      GL_UNSIGNED_INT, &(m_Indices2[0]) );

      glDisableClientState( GL_VERTEX_ARRAY );


      {
	glLineWidth(1.0f);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

	{
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDisable(GL_CULL_FACE);
      }

    } // if ( flags & eCellShading_bound )


  /**
  {
    glColor3f(1,0,1);
    glPointSize(10);

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, &(m_Vertices[0].x) );

    glDrawArrays( GL_POINTS, 0, m_Vertices.size() );

    glDisableClientState( GL_VERTEX_ARRAY );
  }
  //*/

  /**
  {
    m_sec_Vertices.clear();

    for (unsigned int i = 0; i < m_Vertices.size(); ++i)
      {
	m_sec_Vertices.push_back( m_Vertices[ i ] );
	m_sec_Vertices.push_back( m_Vertices[ i ] + m_Normales[ i ] );
      }

    glColor3f(1,0,1);
    // glPointSize(10);

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, &(m_sec_Vertices[0].x) );

    glDrawArrays( GL_LINES, 0, m_sec_Vertices.size() );

    glDisableClientState( GL_VERTEX_ARRAY );

  }
  //*/

}



void	Renderer::CastShadow(const sf::Vector3f& lightPosition)
{
  glDisable(GL_LIGHTING);


  { /// Setup shadow volume

    m_ShadowVertices.clear();

    sf::Vector3f*	cur_vert[3];
    sf::Vector3f	far_vert[3];

    unsigned int i = 0;
    while (i < m_Indices.size())
      {

	cur_vert[0] = &( m_Vertices[ m_Indices[ i + 0 ] ] );
	cur_vert[1] = &( m_Vertices[ m_Indices[ i + 1 ] ] );
	cur_vert[2] = &( m_Vertices[ m_Indices[ i + 2 ] ] );

	// if ( IsVisible( *cur_vert[ 0 ],
	// 		*cur_vert[ 1 ],
	// 		*cur_vert[ 2 ],
	// 		lightPosition ) )
	if ( IsVisible( (sf::Vector3f**)cur_vert, lightPosition ) )
	  {
	    m_ShadowVertices.push_back( *cur_vert[0] );
	    m_ShadowVertices.push_back( *cur_vert[1] );
	    m_ShadowVertices.push_back( *cur_vert[2] );

	    for (int j = 0; j < 3; ++j)
	      {
		far_vert[j].x = cur_vert[j]->x + (cur_vert[j]->x - lightPosition.x);
		far_vert[j].y = cur_vert[j]->y + (cur_vert[j]->y - lightPosition.y);
		far_vert[j].z = cur_vert[j]->z + (cur_vert[j]->z - lightPosition.z);

		m_ShadowVertices.push_back( far_vert[j] );
	      }

	    for (int j = 0; j < 3; ++j)
	      {
		unsigned int	p1 = j;
		unsigned int	p2 = (j + 1) % 3;

		m_ShadowVertices.push_back( *cur_vert[p1] );
		m_ShadowVertices.push_back( far_vert[p2] );
		m_ShadowVertices.push_back( *cur_vert[p2] );

		m_ShadowVertices.push_back( *cur_vert[p1] );
		m_ShadowVertices.push_back( far_vert[p1] );
		m_ShadowVertices.push_back( far_vert[p2] );
	      }

	  }

	i += 3;
      }

  } /// /Setup shadow volume


  /**
  { /// Render shadow volume

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_BLEND);
    glColor4f(1,1,1, 0.25);

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, &(m_ShadowVertices[0].x) );

    glDrawArrays( GL_TRIANGLES, 0, m_ShadowVertices.size() );

    glDisableClientState( GL_VERTEX_ARRAY );

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  } /// /Render shadow volume
  //*/


  {
    glEnable(GL_DEPTH_TEST); // Activate the depth test

    // glEnable(GL_CULL_FACE); // Activate the culling
    glDisable(GL_CULL_FACE); // Activate the culling

    glCullFace(GL_BACK);   // We are drawing front face

    glDisable(GL_TEXTURE_2D); // no texture here
    glDisable(GL_BLEND);   // no blending


    glClear(GL_STENCIL_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);


    // We change the z-testing function to LESS, to avoid little bugs in shadow
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glStencilFunc(GL_ALWAYS, 1, 0xffffffff);


    /**
    { // first pass, stencil operation decreases stencil value

      // glCullFace(GL_FRONT); // We are drawing the back faces first

      glStencilOp(GL_KEEP, GL_INCR, GL_KEEP); // We increment if the depth test fails

      // DrawShadowVolume();
      { /// Render shadow volume

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, &(m_ShadowVertices[0].x) );

	glDrawArrays( GL_TRIANGLES, 0, m_ShadowVertices.size() );

	glDisableClientState( GL_VERTEX_ARRAY );

      } /// /Render shadow volume


    } // first pass, stencil operation decreases stencil value
    //*/

    /**/
    { // second pass, stencil operation increases stencil value

      glCullFace(GL_BACK); // We are now drawing the front faces

      glStencilOp(GL_KEEP, GL_DECR, GL_KEEP); // We decrement if the depth test fails

      // DrawShadowVolume();
      { /// Render shadow volume

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, &(m_ShadowVertices[0].x) );

	glDrawArrays( GL_TRIANGLES, 0, m_ShadowVertices.size() );

	glDisableClientState( GL_VERTEX_ARRAY );

      } /// /Render shadow volume

    } // second pass, stencil operation increases stencil value
    //*/



    { //draw a shadowing rectangle covering the entire screen

      // glFrontFace(GL_CCW);
      // glFrontFace(GL_CW);


      glDisable(GL_CULL_FACE);


      glColorMask(1, 1, 1, 1);

      // glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
      // glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      // glColor4fv(color);
      glColor4f(1,0,0, 1);


      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
      // glStencilFunc(GL_EQUAL, 0x0, 0xffffffff);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


      /**/
      {
	glPushMatrix();
	{
	  glLoadIdentity();

	  glBegin(GL_TRIANGLE_STRIP);
	  {
	    glVertex3f(-0.1f,  0.1f, -0.10f);
	    glVertex3f(-0.1f, -0.1f, -0.10f);
	    glVertex3f( 0.1f,  0.1f, -0.10f);
	    glVertex3f( 0.1f, -0.1f, -0.10f);
	  }
	  glEnd();
	}
	glPopMatrix();
      }
      //*/


      glDisable(GL_BLEND);

    } //draw a shadowing rectangle covering the entire screen



    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDisable(GL_STENCIL_TEST);

    glDisable(GL_CULL_FACE);
  }

}
