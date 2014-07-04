

#ifndef MD3PLAYER_HH
#define MD3PLAYER_HH


#include "Md3Model.hh"
#include "Md3PlayerSkin.hh"
// #include "../TextureManager.hh"
#include "../RessourceManager.hh"


// class	Md3Weapon;


enum Md3PlayerLOD
  {
    kLodLow,
    kLodMedium,
    kLodHigh,

    kLodDefault = kLodHigh,
  };


enum Md3PlayerAnimType
  {
    kBothDeath1,
    kBothDead1,
    kBothDeath2,
    kBothDead2,
    kBothDeath3,
    kBothDead3,


    kTorsoGesture,

    kTorsoAttack,
    kTorsoAttack2,

    kTorsoDrop,
    kTorsoRaise,

    kTorsoStand,
    kTorsoStand2,


    kLegsWalkCr,
    kLegsWalk,
    kLegsRun,
    kLegsBack,
    kLegsSwim,

    kLegsJump,
    kLegsLand,

    kLegsJumpB,
    kLegsLandB,

    kLegsIdle,
    kLegsIdleCr,

    kLegsTurn,


    kMaxAnimations
  };


struct Md3PlayerAnim_t
{
  int first;
  int num;
  int looping;
  int fps;
};



class Md3Player
{
public:

  Md3Player (const std::string &path, Md3PlayerLOD lod = kLodDefault)
    throw (Md3Exception);
  ~Md3Player ();

private:

  struct Md3AnimState
  {
  public:

    Md3AnimState ();

    void setup (Md3PlayerAnim_t *a);
    void update (float dt);

  private:

    Md3PlayerAnim_t *anim;

    float curr_time;
    float old_time;

  public:

    int curr_frame;
    int next_frame;
    float interp;

  };


public:

  typedef std::map<std::string, Md3PlayerSkin*> SkinMap;

public:

  void draw () const;
  void	draw (int curr_frame, int next_frame, float interp,
	      int curr_frame2, int next_frame2, float interp2) const;


  void animate (float dt);

  void setScale (float scale) { _scale = scale; }
  void setAnimation (Md3PlayerAnimType type);
  void setSkin (const std::string &name);


  float scale () const { return _scale; }
  const std::string &path () const { return _path; }
  const std::string &name () const { return _name; }
  Md3PlayerLOD lod () const { return _lod; }
  const SkinMap &skins () const { return _skins; }
  const std::string &skinName () const { return _currentSkinName; }

  const	Md3PlayerAnim_t	GetAnim(Md3PlayerAnimType anim)
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

  Md3Model* _lower;
  Md3Model* _upper;
  Md3Model* _head;

  Md3AnimState _lowerAnim;
  Md3AnimState _upperAnim;

  Md3PlayerAnim_t _anims[kMaxAnimations];

  SkinMap _skins;
  Md3PlayerSkin *_currentSkin;
  std::string _currentSkinName;

  // Md3Weapon *_weapon;

  std::string _path;
  std::string _name;
  float _scale;
  Md3PlayerLOD _lod;

};




#endif
