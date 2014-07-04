

#ifndef MD3ALIENSKIN_HH
#define MD3ALIENSKIN_HH


#include "Md3Model.hh"
#include "Md3Exception.hh"


#include <string>


class Md3AlienSkin
{
public:

  Md3AlienSkin (const std::string &path, const std::string &name)
    throw (Md3Exception);
  ~Md3AlienSkin ();

public:

  void setModelTextures (Md3Model *model);

  const std::string &path () const;
  const std::string &name () const;

private:

  typedef std::map<std::string, const sf::Texture *> ImgMap;

private:

  void loadSkinFile (const std::string &filename, ImgMap &tmap)
    throw (Md3Exception);

  void setModelTextures (Md3Model *model, const ImgMap &tmap) const;

private:

  ImgMap _modelTextures;

  std::string _path;
  std::string _name;

};


#endif

