

#ifndef MD3MODEL_HH
#define MD3MODEL_HH


#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>


// #include "../Math/Mathlib.hpp"
// #include "../Math/Matrix4x4.hpp"
// #include "../Math/Quaternion.hpp"
// #include "../Math/Vector3.hpp"

#include "../RessourceManager.hh"

#include "Md3Exception.hh"

#include "NormalLookupTable.hh"

#include "Md3Mesh.hh"


typedef GLfloat vec3_t[3];


struct Md3Header_t
{
  int ident;
  int version;

  char name[64];
  int flags;

  int num_frames;
  int num_tags;
  int num_meshes;
  int num_skins;

  int offset_frames;
  int offset_tag;
  int offset_meshes;
  int offset_eof;
};


struct Md3Frame_t
{
  vec3_t min_bounds;
  vec3_t max_bounds;
  vec3_t local_origin;

  float radius;
  char creator[16];
};


struct Md3Tag_t
{
  char name[64];
  vec3_t origin;
  float axis[3][3];
};


class Md3Model
{
public:

  Md3Model (const std::string &filename)
    throw (Md3Exception);
  ~Md3Model ();

private:

  struct Md3QuaternionTag
  {
    Md3QuaternionTag (const Md3Tag_t &tag);

    std::string name;
    Vector3f origin;
    Quaternionf orient;
  };

public:

  void loadShaders () const;
  void draw () const;

  // void renderFrameImmediate (int frame) const;
  void renderFrameItpWithVertexArrays( int frameA, int frameB,
				       float interp ) const;

  void	renderFrameItpWithrenderer( int frameA, int frameB,
				    float interp ) const;


  bool link (const std::string &name, const Md3Model *model);
  bool unlink (const std::string &name);
  void setScale (float scale) { _scale = scale; }

  void setTexture (const std::string &mesh, const sf::Texture *tex);

  void setupAnimation (int currFrame, int nextFrame, float interp);

  int numMeshes () const { return _header.num_meshes; }
  int numFrames () const { return _header.num_frames; }
  int numTags () const { return _header.num_tags; }
  float scale () const { return _scale; }
  const std::string &name () const { return _name; }

private:

  static const int _kMd3Ident;
  static const int _kMd3Version;

  Md3Header_t _header;

  std::vector<Md3Frame_t*> _frames;
  std::vector<Md3Mesh*> _meshes;
  std::vector<Md3QuaternionTag*> _qtags;
  std::vector<const Md3Model*> _links;

  int _currFrame;
  int _nextFrame;
  float _interp;

  GLfloat _scale;
  std::string _name;

};


#endif

