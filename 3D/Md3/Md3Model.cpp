

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>


#include "Md3Model.hh"


Md3Model::Md3QuaternionTag::Md3QuaternionTag (const Md3Tag_t &tag)
  : name (tag.name)
{
  origin._x = tag.origin[0];
  origin._y = tag.origin[1];
  origin._z = tag.origin[2];

  Matrix4x4f m;
  m._m11 = tag.axis[0][0]; m._m12 = tag.axis[0][1]; m._m13 = tag.axis[0][2];
  m._m21 = tag.axis[1][0]; m._m22 = tag.axis[1][1]; m._m23 = tag.axis[1][2];
  m._m31 = tag.axis[2][0]; m._m32 = tag.axis[2][1]; m._m33 = tag.axis[2][2];

  orient.fromMatrix (m);
  orient.normalize ();
}


const int Md3Model::_kMd3Ident = 'I' + ('D'<<8) + ('P'<<16) + ('3'<<24);

const int Md3Model::_kMd3Version = 15;






Md3Model::Md3Model (const std::string &filename) throw (Md3Exception)
  : _currFrame (0), _nextFrame (0), _interp (0.0f),
    _scale (1.0f), _name (filename)
{
  std::fstream ifs (filename.c_str(), std::ios::in | std::ios::binary);

  if (ifs.fail ())
    throw Md3Exception ("Md3Model::Md3Model : Couldn't open file", filename);

  ifs.read (reinterpret_cast<char *>(&_header),
	    sizeof (Md3Header_t));

  if (_header.ident != _kMd3Ident)
    {
      ifs.close();
      throw Md3Exception ("Md3Model::Md3Model : Bad file ident", filename);
    }

  if (_header.version != _kMd3Version)
    {
      ifs.close();
      throw Md3Exception ("Md3Model::Md3Model : Bad file version", filename);
    }

  _meshes.reserve (_header.num_meshes);
  _qtags.reserve (_header.num_tags * _header.num_frames);
  _links.reserve (_header.num_tags);

  Md3Frame_t frame;
  ifs.seekg (_header.offset_frames, std::ios::beg);
  for (int i = 0; i < _header.num_frames; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&frame), sizeof (Md3Frame_t));
      _frames.push_back(new Md3Frame_t(frame));
    }

  Md3Tag_t tag;
  ifs.seekg (_header.offset_tag, std::ios::beg);
  for (int i = 0; i < _header.num_tags * _header.num_frames; ++i)
    {
      ifs.read (reinterpret_cast<char *>(&tag), sizeof (Md3Tag_t));
      _qtags.push_back(new Md3QuaternionTag(tag));
    }

  ifs.seekg (_header.offset_meshes, std::ios::beg);
  for (int i = 0; i < _header.num_meshes; ++i)
    _meshes.push_back(new Md3Mesh(ifs, _header.num_frames));

  ifs.close();

  for (int i = 0; i < _header.num_tags; ++i)
    _links.push_back (NULL);

}


Md3Model::~Md3Model ()
{
  for (std::vector<Md3Frame_t*>::iterator itr = _frames.begin(); itr != _frames.end(); ++itr)
    delete *itr;

  for (std::vector<Md3Mesh*>::iterator itr = _meshes.begin(); itr != _meshes.end(); ++itr)
    delete *itr;

  for (std::vector<Md3QuaternionTag*>::iterator itr = _qtags.begin(); itr != _qtags.end(); ++itr)
    delete *itr;
}


void	Md3Model::loadShaders () const
{
  for (int i = 0; i < _header.num_meshes; ++i)
    _meshes[i]->loadShader (0);
}


void	Md3Model::draw () const
{
  Matrix4x4f m;

  renderFrameItpWithVertexArrays(_currFrame, _nextFrame, _interp);

  for (int i = 0; i < _header.num_tags; ++i)
    {
      if (_links[i] == NULL)
  	continue;

      const Quaternionf &qA = _qtags[_currFrame * _header.num_tags + i]->orient;
      const Quaternionf &qB = _qtags[_nextFrame * _header.num_tags + i]->orient;

      m.fromQuaternion( Slerp(qA, qB, _interp) );

      const Vector3f &currPos = _qtags[_currFrame * _header.num_tags + i]->origin;
      const Vector3f &nextPos = _qtags[_nextFrame * _header.num_tags + i]->origin;

      m.setTranslation( (currPos + _interp * (nextPos - currPos)) * _scale );

      glPushMatrix ();
      {
  	glMultMatrixf (m);
  	_links[i]->draw ();
      }
      glPopMatrix ();
    }
}


// void	Md3Model::renderFrameImmediate (int frame) const
// {
//   if ( (frame < 0) || (frame >= _header.num_frames) )
//     return;

//   for (int i = 0; i < _header.num_meshes; ++i)
//     _meshes[i]->renderFrameImmediate(frame, _scale);
// }


void	Md3Model::renderFrameItpWithVertexArrays( int frameA, int frameB,
						  float interp ) const
{
  int maxFrame = _header.num_frames - 1;

  if ( (frameA < 0) || (frameA > maxFrame) ||
       (frameB < 0) || (frameB > maxFrame) )
    return;

  for (int i = 0; i < _header.num_meshes; ++i)
    {
      _meshes[i]->setupVertexArrays(frameA, frameB, interp, _scale);
      _meshes[i]->renderWithVertexArrays();

      // _meshes[i]->renderFrameImmediate(frameA, _scale);
    }
}


void	Md3Model::renderFrameItpWithrenderer( int frameA, int frameB,
					      float interp ) const
{
  int	maxFrame = _header.num_frames - 1;

  if ( (frameA < 0) || (frameA > maxFrame) ||
       (frameB < 0) || (frameB > maxFrame) )
    return;

  for (int i = 0; i < _header.num_meshes; ++i)
    {
      _meshes[i]->setupRenderer(frameA, frameB, interp, _scale);

      // _meshes[i]->renderWithVertexArrays();
    }
}




bool	Md3Model::link (const std::string &name, const Md3Model *model)
{
  for (int i = 0; i < _header.num_tags; ++i)
    {
      if (name == _qtags[i]->name)
	{
	  _links[i] = model;
	  return true;
	}
    }

  return false;
}


bool	Md3Model::unlink (const std::string &name)
{
  for (int i = 0; i < _header.num_tags; ++i)
    {
      if (name == _qtags[i]->name)
	{
	  _links[i] = NULL;
	  return true;
	}
    }

  return false;
}


void	Md3Model::setTexture (const std::string &mesh, const sf::Texture *tex)
{
  for (int i = 0; i < _header.num_meshes; ++i)
    {
      std::string test(_meshes[i]->name());
      if (test.substr(0, mesh.size()) == mesh)
	_meshes[i]->setTexture (tex);
    }
}


void	Md3Model::setupAnimation (int currFrame, int nextFrame, float interp)
{
  _currFrame = currFrame;
  _nextFrame = nextFrame;
  _interp = interp;

  if (_currFrame >= _header.num_frames)
    _currFrame = _header.num_frames - 1;

  if (_nextFrame >= _header.num_frames)
    _nextFrame = _header.num_frames - 1;
}
