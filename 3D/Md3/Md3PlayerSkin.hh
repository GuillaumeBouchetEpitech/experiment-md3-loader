

#ifndef MD3PLAYERSKIN_HH
#define MD3PLAYERSKIN_HH


#include "Md3Model.hh"
#include "Md3Exception.hh"


class Md3PlayerSkin
{
public:

  Md3PlayerSkin (const std::string &path, const std::string &name)
    throw (Md3Exception);
  ~Md3PlayerSkin ();

public:

  void setLowerTextures (Md3Model *model);
  void setUpperTextures (Md3Model *model);
  void setHeadTextures (Md3Model *model);

  const std::string &path () const;
  const std::string &name () const;

private:

  typedef std::map<std::string, const sf::Texture *> ImgMap;

private:

  void loadSkinFile (const std::string &filename, ImgMap &tmap)
    throw (Md3Exception);

  void setModelTextures (Md3Model *model, const ImgMap &tmap) const;

private:

  ImgMap _lowerTextures;
  ImgMap _upperTextures;
  ImgMap _headTextures;

  std::string _path;
  std::string _name;
};


#endif

