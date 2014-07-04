
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

#include "Md3Player.hh"

// #include "Md3Weapon.hh"


Md3Player::Md3AnimState::Md3AnimState ()
  : anim (NULL), curr_time (0.0f), old_time (0.0f),
    curr_frame (0), next_frame (0), interp (0.0f)
{
}


void	Md3Player::Md3AnimState::setup (Md3PlayerAnim_t *a)
{
  anim = a;
  curr_frame = a->first;
  next_frame = a->first;
  interp = 0.0f;
}


void	Md3Player::Md3AnimState::update (float dt)
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





Md3Player::Md3Player (const std::string &path, Md3PlayerLOD lod)
  throw (Md3Exception)
  : //_weapon (NULL),
    _path (path), _scale (1.0f), _lod (lod)
{
  if (_path.find_last_of ('/') < _path.length () - 1)
    _path += '/';

  _name.assign (_path, 0, _path.find_last_of ('/'));
  _name.assign (_name, _name.find_last_of ('/') + 1, _name.length ());

  loadModels (_path);

  loadAnimations (_path);

  loadSkins (_path);





  // std::cout << std::endl;
  // std::cout << "GETANIM" << std::endl;
  // for (int i = 0 ; i < kMaxAnimations; ++i)
  //   {
  //     std::cout << std::endl;
  //     std::cout << _anims[i].first << "  "
  // 		<< _anims[i].num << "  "
  // 		<< _anims[i].looping << "  "
  // 		<< _anims[i].fps << std::endl;
  //   }





}


Md3Player::~Md3Player ()
{
  delete _lower;
  delete _upper;
  delete _head;

  for (SkinMap::iterator itr = _skins.begin(); itr != _skins.end(); ++itr)
    delete itr->second;
}


void	Md3Player::draw () const
{
  _lower->setScale (_scale);
  _upper->setScale (_scale);
  _head->setScale (_scale);

  // if (_weapon)
  //   _weapon->scaleModels ();

  if (_currentSkin)
    {
      _currentSkin->setLowerTextures (_lower);
      _currentSkin->setUpperTextures (_upper);
      _currentSkin->setHeadTextures (_head);
    }

  const Md3AnimState &low = _lowerAnim;
  const Md3AnimState &upp = _upperAnim;

  _lower->setupAnimation (low.curr_frame, low.next_frame, low.interp);
  _upper->setupAnimation (upp.curr_frame, upp.next_frame, upp.interp);

  // glPushMatrix ();
  // {
    // glRotatef (-90.0, 1.0, 0.0, 0.0);
    // glRotatef (-90.0, 0.0, 0.0, 1.0);

    _lower->draw ();
  // }
  // glPopMatrix ();
}

void    Md3Player::draw (int curr_frame, int next_frame, float interp,
			 int curr_frame2, int next_frame2, float interp2) const
{
  _lower->setScale (_scale);
  _upper->setScale (_scale);
  _head->setScale (_scale);

  // if (_weapon)
  //   _weapon->scaleModels ();

  if (_currentSkin)
    {
      _currentSkin->setLowerTextures (_lower);
      _currentSkin->setUpperTextures (_upper);
      _currentSkin->setHeadTextures (_head);
    }

  _lower->setupAnimation (curr_frame2, next_frame2, interp2);
  _upper->setupAnimation (curr_frame, next_frame, interp);

  _lower->draw ();
}



// void	Md3Player::renderFrame (int upperFrame, int lowerFrame) const
// {
//   _lower->setScale (_scale);
//   _upper->setScale (_scale);
//   _head->setScale (_scale);

//   // if (_weapon)
//   //   _weapon->scaleModels ();

//   if (_currentSkin)
//     {
//       _currentSkin->setLowerTextures (_lower);
//       _currentSkin->setUpperTextures (_upper);
//       _currentSkin->setHeadTextures (_head);
//     }

//   _lower->setupAnimation (lowerFrame, lowerFrame, 0.0f);
//   _upper->setupAnimation (upperFrame, upperFrame, 0.0f);

//   glPushMatrix ();
//   {
//     // glRotatef (-90.0, 1.0, 0.0, 0.0);
//     // glRotatef (-90.0, 0.0, 0.0, 1.0);

//     _lower->draw ();
//   }
//   glPopMatrix ();
// }


void	Md3Player::animate (float dt)
{
  _lowerAnim.update (dt);
  _upperAnim.update (dt);
}


void	Md3Player::setAnimation (Md3PlayerAnimType type)
{
  if ((type >= kBothDeath1) && (type <= kBothDead3))
    {
      _lowerAnim.setup (&_anims[type]);
      _upperAnim.setup (&_anims[type]);
    }
  else if ((type >= kTorsoGesture) && (type <= kTorsoStand2))
    {
      _upperAnim.setup (&_anims[type]);
    }
  else if ((type >= kLegsWalkCr) && (type <= kLegsTurn))
    {
      _lowerAnim.setup (&_anims[type]);
    }
}


void	Md3Player::setSkin (const std::string &name)
{
  SkinMap::iterator itor = _skins.find (name);
  if (itor != _skins.end ())
    {
      _currentSkin = itor->second;
      _currentSkinName = itor->first;
    }
  else
    _currentSkin = NULL;
}


// void	Md3Player::linkWeapon (Md3Weapon *weapon)
// {
//   _weapon = weapon;

//   if (_weapon)
//     _weapon->linkToModel (_upper);
// }


// void	Md3Player::unlinkWeapon ()
// {
//   if (_weapon)
//     {
//       _upper->unlink ("tag_weapon");
//       _weapon = NULL;
//     }
// }


void	Md3Player::loadModels(const std::string &path)
  throw (Md3Exception)
{
  std::string suffix, modelpath;

  switch (_lod)
    {
    case kLodLow:
      suffix = "_2.md3";
      break;

    case kLodMedium:
      suffix = "_1.md3";
      break;

    case kLodHigh:
      suffix = ".md3";
      break;
    }

  modelpath = path + "lower" + suffix;
  _lower = new Md3Model(modelpath);

  modelpath = path + "upper" + suffix;
  _upper = new Md3Model(modelpath);

  modelpath = path + "head" + suffix;
  _head = new Md3Model(modelpath);


  _lower->link ("tag_torso", _upper);
  _upper->link ("tag_head", _head);
}


void	Md3Player::loadAnimations (const std::string &path)
  throw (Md3Exception)
{
  std::string filename (path + "animation.cfg");
  std::string token, buffer;
  int index = 0;

  std::fstream ifs (filename.c_str(), std::ios::in | std::ios::binary);

  if (ifs.fail ())
    throw Md3Exception ("Couldn't open animation file", filename);

  // std::cerr << std::endl;

  while (!ifs.eof ())
    {
      std::getline (ifs, buffer);
      std::istringstream line (buffer);


      // std::cerr << buffer << std::endl;


      char c = line.peek ();

      if (std::isdigit (c))
	{
	  line >> _anims[index].first;
	  line >> _anims[index].num;
	  line >> _anims[index].looping;
	  line >> _anims[index].fps;

	  // std::cerr << _anims[index].first << "  "
	  // 	    << _anims[index].num << "  "
	  // 	    << _anims[index].looping << "  "
	  // 	    << _anims[index].fps << std::endl;

	  index++;
	}
    }

  ifs.close ();

  int skip = _anims[kLegsWalkCr].first - _anims[kTorsoGesture].first;

  for (int i = kLegsWalkCr; i < kMaxAnimations; ++i)
    _anims[i].first -= skip;
}


void	Md3Player::loadSkins (const std::string &path)
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
      const std::string pattern ("head_");

      if (filename.length () - fileext.length () == filename.find (fileext))
	{
	  if (filename.compare (0, pattern.length (), pattern) == 0)
	    {
	      const std::string name (filename, pattern.length (), filename.find (fileext) - pattern.length ());

	      Md3PlayerSkin* skin = new Md3PlayerSkin (path, name);
	      _skins.insert (SkinMap::value_type (name, skin));
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



