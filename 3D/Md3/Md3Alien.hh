

#ifndef MD3ALIEN_HH
#define MD3ALIEN_HH


#include "Md3Model.hh"
#include "Md3AlienSkin.hh"
// #include "../TextureManager.hh"
#include "../RessourceManager.hh"


// class	Md3Weapon;



enum Md3AlienAnimType
  {
    kAStand,

    kAGesture,

    kAWalk,
    kARun,
    kARunBack,
    kACharge,

    kARunLeft,
    kAWalkLeft,
    kARunRight,
    kAWalkRight,

    kASwim,

    kAJump,
    kALand,
    kAJumpBack,
    kALandBack,

    kATurn,

    kAAttack1,
    kAAttack2,
    kAAttack3,

    kAPain1,
    kAPain2,

    kADeath1,
    kADead1,
    kADeath2,
    kADead2,
    kADeath3,
    kADead3,

    kAMaxAnimations,
  };


struct Md3AlienAnim_t
{
  int first;
  int num;
  int looping;
  int fps;
};



class Md3Alien
{
public:

  Md3Alien(const std::string &path)
    throw (Md3Exception);
  ~Md3Alien();

private:

  struct Md3AlienAnimState
  {
  public:

    Md3AlienAnimState ();

    void setup (Md3AlienAnim_t *a);
    void update (float dt);

  private:

    Md3AlienAnim_t *anim;

    float curr_time;
    float old_time;

  public:

    int curr_frame;
    int next_frame;
    float interp;

  };


public:

  typedef std::map<std::string, Md3AlienSkin*> SkinAlienMap;

public:

  void draw () const;
  void draw (int curr_frame, int next_frame, float interp) const;

  void draw2(int curr_frame, int next_frame, float interp) const;

  void animate (float dt);

  void setScale (float scale) { _scale = scale; }
  void setAnimation (Md3AlienAnimType type);
  void setSkin (const std::string &name);

  float scale () const { return _scale; }

  const SkinAlienMap &skins () const { return _skins; }

  Md3AlienAnim_t GetAnim(Md3AlienAnimType anim) const
  {
    return (_anims[anim]);
  }


private:

  void loadModels (const std::string &path)
    throw (Md3Exception);

  void loadAnimations (const std::string &path)
    throw (Md3Exception);

  void loadSkins (const std::string &path)
    throw (Md3Exception);

private:

  Md3Model* _model;

  Md3AlienAnimState _modelAnim;

  Md3AlienAnim_t _anims[kAMaxAnimations];

  SkinAlienMap _skins;
  Md3AlienSkin *_currentSkin;
  std::string _currentSkinName;

  // Md3Weapon *_weapon;

  std::string _path;
  std::string _name;
  float _scale;

};




#endif
