

#include "Md3PlayerSkin.hh"


Md3PlayerSkin::Md3PlayerSkin (const std::string &path, const std::string &name)
  throw (Md3Exception)
  : _path (path), _name (name)
{
  loadSkinFile (path + "lower_" + name + ".skin", _lowerTextures);

  loadSkinFile (path + "upper_" + name + ".skin", _upperTextures);

  loadSkinFile (path + "head_" + name + ".skin", _headTextures);
}

Md3PlayerSkin::~Md3PlayerSkin ()
{
}

void Md3PlayerSkin::setLowerTextures(Md3Model *model)
{
  setModelTextures (model, _lowerTextures);
}

void Md3PlayerSkin::setUpperTextures(Md3Model *model)
{
  setModelTextures (model, _upperTextures);
}

void Md3PlayerSkin::setHeadTextures(Md3Model *model)
{
  setModelTextures (model, _headTextures);
}


const std::string&	Md3PlayerSkin::path () const
{
  return _path;
}

const std::string&	Md3PlayerSkin::name () const
{
  return _name;
}


void	Md3PlayerSkin::loadSkinFile (const std::string &filename, ImgMap &tmap)
  throw (Md3Exception)
{
  std::fstream ifs (filename.c_str(), std::ios::in | std::ios::binary);

  if (ifs.fail ())
    throw Md3Exception ("Couldn't open skin file", filename);

  RessourceManager* RManager = RessourceManager::GetInstance();

  while (!ifs.eof ())
    {
      std::string meshname, texname, buffer;

      std::getline (ifs, meshname, ',');
      ifs >> texname;

      if (!meshname.empty () && !texname.empty ()
	  && meshname.compare (0, 4, "tag_") != 0)
	{
	  long start = texname.find_last_of ('/') + 1;
	  long end = texname.length () - start;
	  texname.assign (texname, start, end);
	  texname = _path + texname;

	  sf::Texture *tex = RManager->LoadTexture(texname, false);
	  tmap.insert (ImgMap::value_type (meshname, tex));
	}

      std::getline (ifs, buffer);
    }

  ifs.close ();
}

void	Md3PlayerSkin::setModelTextures (Md3Model *model, const ImgMap &tmap) const
{
  for (ImgMap::const_iterator itor = tmap.begin();
       itor != tmap.end();
       ++itor)
    model->setTexture (itor->first, itor->second);
}



