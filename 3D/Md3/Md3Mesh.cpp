

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

#include "Md3Model.hh"

#include "../Renderer.hpp"


const NormalLookupTable Md3Mesh::_kAnorms;
const int Md3Mesh::_kMd3Ident = 'I' + ('D'<<8) + ('P'<<16) + ('3'<<24);


vec3_t Md3Mesh::_kVertexArray[kMd3MaxTriangles * 3];
vec3_t Md3Mesh::_kNormalArray[kMd3MaxTriangles * 3];


Md3Mesh::Md3Mesh(std::fstream &ifs, int max_anim) throw (Md3Exception)
  : _tex (NULL)
{
  if (!ifs.is_open ())
    throw Md3Exception ("Stream not opened");

  long pos = ifs.tellg ();

  ifs.read (reinterpret_cast<char *>(&_header),
	    sizeof (Md3MeshHeader_t));

  if (_header.ident != _kMd3Ident)
    throw Md3Exception ("Bad mesh ident");

  _shaders.reserve (_header.num_shaders);
  _triangles.reserve (_header.num_triangles);
  _texCoords.reserve (_header.num_verts);
  _vertices.reserve (_header.num_verts * _header.num_frames);
  _indices.reserve (_header.num_triangles * 3);

  Md3Shader_t shader;
  ifs.seekg (pos + _header.offset_shaders, std::ios::beg);
  for (int i = 0; i < _header.num_shaders; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&shader), sizeof (Md3Shader_t));
      _shaders.push_back (new Md3Shader_t(shader));
    }

  Md3Triangle_t tri;
  ifs.seekg (pos + _header.offset_triangles, std::ios::beg);
  for (int i = 0; i < _header.num_triangles; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&tri), sizeof (Md3Triangle_t));
      _triangles.push_back(new Md3Triangle_t(tri));
    }

  Md3TexCoord_t st;
  ifs.seekg (pos + _header.offset_st, std::ios::beg);
  for (int i = 0; i < _header.num_verts; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&st), sizeof (Md3TexCoord_t));
      _texCoords.push_back (st);
    }

  Md3Vertex_t vert;
  ifs.seekg (pos + _header.offset_xyznormal, std::ios::beg);
  for (int i = 0; i < _header.num_verts * _header.num_frames; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&vert), sizeof (Md3Vertex_t));
      _vertices.push_back(new Md3Vertex_t(vert));
    }

  ifs.seekg (pos + _header.offset_end, std::ios::beg);

  for (int i = 0; i < _header.num_triangles; ++i)
    for (int j = 2; j >= 0; --j)
      _indices.push_back (_triangles[i]->index[j]);


  _glists.resize(max_anim, 0);
}


Md3Mesh::~Md3Mesh ()
{
  for (std::vector<Md3Shader_t*>::iterator itr = _shaders.begin(); itr != _shaders.end(); ++itr)
    delete *itr;

  for (std::vector<Md3Triangle_t*>::iterator itr = _triangles.begin(); itr != _triangles.end(); ++itr)
    delete *itr;

  for (std::vector<Md3Vertex_t*>::iterator itr = _vertices.begin(); itr != _vertices.end(); ++itr)
    delete *itr;
}


void	Md3Mesh::loadShader (int index)
{
  if (index < 0 || index >= _header.num_shaders)
    return;

  const std::string filename (_shaders[index]->name);
  if (!filename.empty ())
    {
      RessourceManager* RManager = RessourceManager::GetInstance();
      setTexture(RManager->LoadTexture(filename));
    }
}


void	Md3Mesh::bindTexture () const
{
  if (!_tex)
    {
      glBindTexture (GL_TEXTURE_2D, 0);
      return;
    }

  sf::Texture::bind(_tex);
}


void	Md3Mesh::setupVertexArrays (int frameA, int frameB, float interp, float scale)
{
  int frameOffsetA = frameA * _header.num_verts;
  int frameOffsetB = frameB * _header.num_verts;

  float s = scale * kMd3XYZScale;

  for (int i = 0; i < _header.num_verts; ++i)
    {
      const Md3Vertex_t *pVertA = _vertices[frameOffsetA + i];
      const Md3Vertex_t *pVertB = _vertices[frameOffsetB + i];

      {
	const int &uA = pVertA->normal[0];
	const int &vA = pVertA->normal[1];

	const int &uB = pVertB->normal[0];
	const int &vB = pVertB->normal[1];

	const float* normA = _kAnorms[uA][vA];
	const float* normB = _kAnorms[uB][vB];

	_kNormalArray[i][0] = normA[0] + interp * (normB[0] - normA[0]);
	_kNormalArray[i][1] = normA[1] + interp * (normB[1] - normA[1]);
	_kNormalArray[i][2] = normA[2] + interp * (normB[2] - normA[2]);
      }

      {
	const short *vertA = pVertA->v;
	const short *vertB = pVertB->v;

	_kVertexArray[i][0] = static_cast<float>(vertA[0] + interp * (vertB[0] - vertA[0])) * s;
	_kVertexArray[i][1] = static_cast<float>(vertA[1] + interp * (vertB[1] - vertA[1])) * s;
	_kVertexArray[i][2] = static_cast<float>(vertA[2] + interp * (vertB[2] - vertA[2])) * s;
      }
    }
}




void	VMatMult(float *M, sf::Vector3f& vec)
{
  static float	v[4];
  static float	res[4];

  v[0] = vec.x;
  v[1] = vec.y;
  v[2] = vec.z;
  v[3] = 1.0f;

  res[ 0 ] = M[ 0 ] * v[ 0 ] + M[ 4 ] * v[ 1 ] + M[  8 ] * v[ 2 ] + M[ 12 ] * v[ 3 ];
  res[ 1 ] = M[ 1 ] * v[ 0 ] + M[ 5 ] * v[ 1 ] + M[  9 ] * v[ 2 ] + M[ 13 ] * v[ 3 ];
  res[ 2 ] = M[ 2 ] * v[ 0 ] + M[ 6 ] * v[ 1 ] + M[ 10 ] * v[ 2 ] + M[ 14 ] * v[ 3 ];
  res[ 3 ] = M[ 3 ] * v[ 0 ] + M[ 7 ] * v[ 1 ] + M[ 11 ] * v[ 2 ] + M[ 15 ] * v[ 3 ];

  vec.x = res[ 0 ];
  vec.y = res[ 1 ];
  vec.z = res[ 2 ];
}


void	Md3Mesh::setupRenderer(int frameA, int frameB, float interp, float scale)
{
  // Renderer::m_pTexture = _tex;
  Renderer::m_pTexture = const_cast<sf::Texture*>(_tex);

  int frameOffsetA = frameA * _header.num_verts;
  int frameOffsetB = frameB * _header.num_verts;

  float s = scale * kMd3XYZScale;

  Renderer::m_Vertices.clear();
  Renderer::m_Normales.clear();
  Renderer::m_TexCoords.clear();
  Renderer::m_Indices.clear();


  float	mat[16];

  {
    glPushMatrix();

    glLoadIdentity();

    // glTranslatef( 0, 0, 1.25f );
    glTranslatef( 0, 0, 1.13f );
    glScalef( 0.05f, 0.05f, 0.05f );

    glGetFloatv(GL_MODELVIEW_MATRIX, mat);

    glPopMatrix();
  }

  for (int i = 0; i < _header.num_verts; ++i)
    {
      const Md3Vertex_t *pVertA = _vertices[frameOffsetA + i];
      const Md3Vertex_t *pVertB = _vertices[frameOffsetB + i];

      {
	const int &uA = pVertA->normal[0];
	const int &vA = pVertA->normal[1];

	const int &uB = pVertB->normal[0];
	const int &vB = pVertB->normal[1];

	const float* normA = _kAnorms[uA][vA];
	const float* normB = _kAnorms[uB][vB];


#define	D_NORMAL_SET(v)	normA[ (v) ] + interp * (normB[ (v) ] - normA[ (v) ])

	Renderer::m_Normales.push_back( sf::Vector3f( D_NORMAL_SET(0),
					    D_NORMAL_SET(1),
					    D_NORMAL_SET(2) ) );

#undef	D_NORMAL_SETTER

      }

      {
	const short *vertA = pVertA->v;
	const short *vertB = pVertB->v;

#define	D_VERTEX_SET(v)	static_cast<float>(vertA[(v)] + interp * (vertB[(v)] - vertA[(v)])) * s

	Renderer::m_Vertices.push_back( sf::Vector3f( D_VERTEX_SET(0),
						      D_VERTEX_SET(1),
						      D_VERTEX_SET(2) ) );

	VMatMult( mat, Renderer::m_Vertices.back() );

#undef	D_VERTEX_SET

      }

    } // for (int i = 0; i < _header.num_verts; ++i)

  {
    for (unsigned int i = 0; i < _texCoords.size(); ++i)
      Renderer::m_TexCoords.push_back( sf::Vector2f( _texCoords[i].s,
						     _texCoords[i].t ) );
  }

  {
    for (unsigned int i = 0; i < _indices.size(); ++i)
      Renderer::m_Indices.push_back( _indices[i] );
  }
}


// void	Md3Mesh::renderFrameImmediate (int frame, float scale)
// {
//   int frameOffset = frame * _header.num_verts;

//   float scale_and_uncompress = scale * kMd3XYZScale;

//   glPushAttrib (GL_POLYGON_BIT);
//   glFrontFace (GL_CW);

//   bindTexture ();


//   if (_glists[frame] == 0)
//     {
//       std::cout << "GL_COMPILE_AND_EXECUTE" << std::endl;
//       _glists[frame] = glGenLists(1);
//       glNewList(_glists[frame], GL_COMPILE_AND_EXECUTE);
//       {



// 	glBegin (GL_TRIANGLES);
// 	for (int i = 0; i < _header.num_triangles; ++i)
// 	  {
// 	    for (int j = 0; j < 3; ++j)
// 	      {
// 		const int &vertIndex = _triangles[i]->index[j];
// 		const Md3Vertex_t *pVert = _vertices[frameOffset + vertIndex];
// 		const Md3TexCoord_t *pTexCoords = &_texCoords[vertIndex];

// 		glTexCoord2f (pTexCoords->s, pTexCoords->t);

// 		const unsigned char &un = pVert->normal[0];
// 		const unsigned char &vn = pVert->normal[1];

// 		glNormal3fv (_kAnorms[un][vn]);

// 		vec3_t v;

// 		v[0] = pVert->v[0] * scale_and_uncompress;
// 		v[1] = pVert->v[1] * scale_and_uncompress;
// 		v[2] = pVert->v[2] * scale_and_uncompress;

// 		glVertex3fv (v);
// 	      }
// 	  }
// 	glEnd ();



//       }
//       glEndList();
//     }
//   else
//     glCallList(_glists[frame]);


//   glPopAttrib ();
// }


void	Md3Mesh::renderWithVertexArrays () const
{
  bindTexture();

  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, _kVertexArray);
  glNormalPointer(GL_FLOAT, 0, _kNormalArray);
  glTexCoordPointer(2, GL_FLOAT, 0, &(_texCoords.front()));

  glDrawElements( GL_TRIANGLES, _header.num_triangles * 3,
		  GL_UNSIGNED_INT, &_indices.front() );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}



