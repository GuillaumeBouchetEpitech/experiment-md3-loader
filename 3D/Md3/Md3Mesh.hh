
#ifndef MD3MESH_HH
#define MD3MESH_HH

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include "../Math/Mathlib.hpp"
#include "../Math/Matrix4x4.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Vector3.hpp"

#include "../RessourceManager.hh"


#include "Md3Exception.hh"

#include "NormalLookupTable.hh"


const int kMd3MaxFrames = 1024;
const int kMd3MaxTags = 16;
const int kMd3MaxMeshes = 32;
const int kMd3MaxShaders = 256;
const int kMd3MaxTriangles = 8192;
const int kMd3MaxVertices = 4096;
const float kMd3XYZScale = 1.0f / 64.0f;


typedef GLfloat vec3_t[3];


struct Md3MeshHeader_t
{
  int ident;
  char name[64];
  int flags;

  int num_frames;
  int num_shaders;
  int num_verts;
  int num_triangles;

  int offset_triangles;
  int offset_shaders;
  int offset_st;
  int offset_xyznormal;
  int offset_end;
};


struct Md3Shader_t
{
  char name[64];
  int shader_index;
};


struct Md3Triangle_t
{
  int index[3];
};


struct Md3TexCoord_t
{
  float	s;
  float	t;
};


struct Md3Vertex_t
{
  short v[3];
  unsigned char normal[2];
};



class Md3Mesh
{
public :

  Md3Mesh (std::fstream &ifs, int max_anim)
    throw (Md3Exception);
  ~Md3Mesh ();

public :

  void loadShader (int index);
  void bindTexture () const;
  void setupVertexArrays (int frameA, int frameB, float interp, float scale);

  void	setupRenderer(int frameA, int frameB, float interp, float scale);

  // void renderFrameImmediate (int frame, float scale);
  void renderWithVertexArrays () const;

  void setTexture (const sf::Texture *tex) { _tex = tex; }

  const char *name () const { return _header.name; }

private :

  static const NormalLookupTable _kAnorms;
  static const int _kMd3Ident;

  static vec3_t _kVertexArray[];
  static vec3_t _kNormalArray[];

  Md3MeshHeader_t _header;

  std::vector<Md3Shader_t*> _shaders;
  std::vector<Md3Triangle_t*> _triangles;
  std::vector<Md3TexCoord_t> _texCoords;
  std::vector<Md3Vertex_t*> _vertices;

  std::vector<GLuint> _indices;

  const sf::Texture*	_tex;


  std::vector<GLuint> _glists;


};

#endif
