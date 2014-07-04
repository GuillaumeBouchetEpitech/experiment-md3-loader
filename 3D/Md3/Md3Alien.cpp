
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

#include "Md3Alien.hh"


Md3Alien::Md3AlienAnimState::Md3AlienAnimState ()
  : anim (NULL), curr_time (0.0f), old_time (0.0f),
    curr_frame (0), next_frame (0), interp (0.0f)
{
}


void	Md3Alien::Md3AlienAnimState::setup (Md3AlienAnim_t *a)
{
  anim = a;
  curr_frame = a->first;
  next_frame = a->first;
  interp = 0.0f;
}


void	Md3Alien::Md3AlienAnimState::update (float dt)
{
  curr_time += dt;

  if ((curr_time - old_time) > (1.0 / anim->fps))
    {
      curr_frame = next_frame;
      next_frame++;

      if (next_frame > (anim->first + anim->num - 1))
	next_frame = anim->first;

      old_time = curr_time;
    }

  interp = anim->fps * (curr_time - old_time);
}





Md3Alien::Md3Alien (const std::string &path)
  throw (Md3Exception)
  : _path (path), _scale (1.0f)
{
  if (_path.find_last_of ('/') < _path.length () - 1)
    _path += '/';

  _name.assign (_path, 0, _path.find_last_of ('/'));
  _name.assign (_name, _name.find_last_of ('/') + 1, _name.length ());

  loadModels (_path);

  loadAnimations (_path);

  loadSkins (_path);
}


Md3Alien::~Md3Alien ()
{
  delete _model;

  for (SkinAlienMap::iterator itr = _skins.begin(); itr != _skins.end(); ++itr)
    delete itr->second;
}


void	Md3Alien::draw () const
{
  _model->setScale (_scale);

  if (_currentSkin)
    _currentSkin->setModelTextures (_model);

  const Md3AlienAnimState&	model = _modelAnim;

  _model->setupAnimation (model.curr_frame, model.next_frame, model.interp);

  _model->draw ();
}

void	Md3Alien::draw(int curr_frame, int next_frame, float interp) const
{
  _model->setScale (_scale);

  if (_currentSkin)
    _currentSkin->setModelTextures (_model);

  _model->setupAnimation(curr_frame, next_frame, interp);

  _model->draw();
}



void	Md3Alien::draw2(int curr_frame, int next_frame, float interp) const
{
  _model->setScale (_scale);

  if (_currentSkin)
    _currentSkin->setModelTextures (_model);

  _model->setupAnimation(curr_frame, next_frame, interp);

  _model->renderFrameItpWithrenderer( curr_frame, next_frame, interp );
}





void	Md3Alien::animate (float dt)
{
  _modelAnim.update (dt);
}


void	Md3Alien::setAnimation (Md3AlienAnimType type)
{
  _modelAnim.setup (&_anims[type]);
}


void	Md3Alien::setSkin (const std::string &name)
{
  SkinAlienMap::iterator itor = _skins.find (name);
  if (itor != _skins.end ())
    {
      _currentSkin = itor->second;
      _currentSkinName = itor->first;
    }
  else
    _currentSkin = NULL;
}

void	Md3Alien::loadModels(const std::string &path)
  throw (Md3Exception)
{
  std::string suffix, modelpath;

  suffix = ".md3";

  modelpath = path + "nonseg" + suffix;
  _model = new Md3Model(modelpath);
}


void	Md3Alien::loadAnimations (const std::string &path)
  throw (Md3Exception)
{
  std::string filename (path + "animation.cfg");
  std::string token, buffer;
  int index = 0;

  std::fstream ifs (filename.c_str(), std::ios::in | std::ios::binary);

  if (ifs.fail ())
    throw Md3Exception ("Couldn't open animation file", filename);

  while (!ifs.eof ())
    {
      std::getline (ifs, buffer);
      std::istringstream line (buffer);

      char c = line.peek ();

      if (std::isdigit (c))
	{
	  line >> _anims[index].first;
	  line >> _anims[index].num;
	  line >> _anims[index].looping;
	  line >> _anims[index].fps;
	  index++;
	}
    }

  ifs.close ();

  // int skip = _anims[kLegsWalkCr].first - _anims[kTorsoGesture].first;

  // for (int i = kLegsWalkCr; i < kMaxAnimations; ++i)
  //   _anims[i].first -= skip;
}


void	Md3Alien::loadSkins (const std::string &path)
  throw (Md3Exception)
{
  DIR *dd = opendir (path.c_str ());
  if (!dd)
    throw Md3Exception ("Couldn't open dir", path);

  dirent *dit;

  while ((dit = readdir (dd)) != NULL)
    {
      const std::string filename (dit->d_name);
      const std::string fileext (".skin");
      const std::string pattern ("nonseg_");

      if (filename.length () - fileext.length () == filename.find (fileext))
	{
	  if (filename.compare (0, pattern.length (), pattern) == 0)
	    {
	      const std::string name (filename, pattern.length (), filename.find (fileext) - pattern.length ());

	      Md3AlienSkin* skin = new Md3AlienSkin (path, name);
	      _skins.insert (SkinAlienMap::value_type (name, skin));
	    }
	}

    }

  closedir (dd);

  setSkin ("default");

  if (_currentSkin == NULL)
    {
      _currentSkin = _skins.begin ()->second;
      _currentSkinName = _skins.begin ()->first;
    }

}



