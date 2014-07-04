

#ifndef	AANIMATION_HH
#define	AANIMATION_HH


#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// #include "Math/Vector3.hpp"


#include "Md3/Md3Alien.hh"
#include "Md3/Md3LodPlayer.hh"

#include "3D/RessourceManager.hh"



#include "3D/Renderer.hpp"



class	ATransformation
{
public :
  ATransformation()
  {
  }

  virtual void	Active() const = 0;

};



class	Translation : public ATransformation
{
private :
  Vector3f	m_Translate;

public :
  Translation(Vector3f Translate)
    : m_Translate(Translate)
  {
  }

  void	Active() const
  {
    if (m_Translate._x != 0 || m_Translate._y != 0 || m_Translate._z != 0)
      glTranslatef(m_Translate._x, m_Translate._y, m_Translate._z);
  }

};



class	Rotation : public ATransformation
{
private :
  float		m_angle;
  Vector3f	m_Vector;

public :
  Rotation(float angle, Vector3f Vector)
    : m_angle(angle), m_Vector(Vector)
  {
  }

  void	Active() const
  {
    if (m_angle != 0 && (m_Vector._x != 0 || m_Vector._y != 0 || m_Vector._z != 0))
      glRotatef(m_angle, m_Vector._x, m_Vector._y, m_Vector._z);
  }

};



class	Scale : public ATransformation
{
private :
  Vector3f	m_Scale;

public :
  Scale(Vector3f Value)
    : m_Scale(Value)
  {
  }

  void	Active() const
  {
    if (m_Scale._x != 0 && m_Scale._y != 0 && m_Scale._z != 0)
      glScalef(m_Scale._x, m_Scale._y, m_Scale._z);
  }

};

class	Objet3D
{
protected :
  std::vector<ATransformation*> m_TranfoList;

public :
  Translation*  AddTranslate(Vector3f Translate);
  Rotation*     AddRotate(float angle, Vector3f Vector);
  Scale*        AddScale(Vector3f Value);

  void  ClearTransformations();

};

class	AAnimation : public Objet3D
{
// protected :

//   int	m_currentFrame;
//   int	m_currentTriangle;

//   std::vector<GLuint>	m_allIndex;
//   std::vector<GLuint>	m_allCSIndex;

public :

  AAnimation();
  virtual ~AAnimation();

  enum e_AAnimationAnimType
    {
      AA_Death,
      AA_Dead,

      AA_Gesture,

      AA_Attack,

      AA_Stand,

      AA_Walk,
      AA_Run,
      AA_Turn,

      AA_MaxAnimations
    };

  struct Md3Anim_t
  {
    int first;
    int num;
    int looping;
    int fps;
  };

  struct	SubAnim_t
  {
    Md3Anim_t	s_anims[2];
  };

  virtual void	SetAnimation(e_AAnimationAnimType Type) = 0;

  virtual void	SetLOD(RessourceManager::e_RscManagerLOD LOD) = 0;

  virtual void	Animate(float time) = 0;
  virtual void	Draw() const = 0;

  // void	DrawCS();

};



class	AnimationPlayer : public AAnimation
{
private :

  Md3LodPlayer	m_Animation;

public :

  AnimationPlayer(const std::string &path)
    : m_Animation(path)
  {
  }

  virtual void	SetAnimation(e_AAnimationAnimType Type)
  {
    switch (Type)
      {
      case AA_Death :	m_Animation.setAnimation(kBothDeath1); break;
      case AA_Dead :	m_Animation.setAnimation(kBothDead1); break;

      case AA_Gesture :	m_Animation.setAnimation(kTorsoGesture);
			m_Animation.setAnimation(kLegsIdle); break;

      case AA_Attack :	m_Animation.setAnimation(kTorsoAttack2);
			m_Animation.setAnimation(kLegsIdle); break;

      case AA_Stand :	m_Animation.setAnimation(kTorsoStand2);
			m_Animation.setAnimation(kLegsIdle); break;

      case AA_Walk :	m_Animation.setAnimation(kTorsoStand2);
			m_Animation.setAnimation(kLegsWalk); break;
      case AA_Run :	m_Animation.setAnimation(kTorsoStand2);
			m_Animation.setAnimation(kLegsRun); break;
      case AA_Turn :	m_Animation.setAnimation(kTorsoStand2);
			m_Animation.setAnimation(kLegsTurn); break;

      default :		m_Animation.setAnimation(kTorsoStand2);
			m_Animation.setAnimation(kLegsIdle);
      }
  }

  Md3Anim_t     GetAnimation(e_AAnimationAnimType Type)
  {
    Md3PlayerAnim_t      Md3PlayerAnim;

    switch (Type)
      {
      case AA_Death :   Md3PlayerAnim = m_Animation.GetAnim(kBothDeath1); break;
      case AA_Dead :    Md3PlayerAnim = m_Animation.GetAnim(kBothDead1); break;

      case AA_Gesture : Md3PlayerAnim = m_Animation.GetAnim(kTorsoGesture); break;

      case AA_Attack :  Md3PlayerAnim = m_Animation.GetAnim(kTorsoAttack2); break;

      case AA_Stand :   Md3PlayerAnim = m_Animation.GetAnim(kTorsoStand2); break;

      case AA_Walk :    Md3PlayerAnim = m_Animation.GetAnim(kTorsoStand2); break;
      case AA_Run :     Md3PlayerAnim = m_Animation.GetAnim(kTorsoStand2); break;
      case AA_Turn :    Md3PlayerAnim = m_Animation.GetAnim(kTorsoStand2); break;

      default :         Md3PlayerAnim = m_Animation.GetAnim(kTorsoStand2);
      }

    AAnimation::Md3Anim_t       anim;
    anim.first = Md3PlayerAnim.first;
    anim.num = Md3PlayerAnim.num;
    anim.looping = Md3PlayerAnim.looping;
    anim.fps = Md3PlayerAnim.fps;

    return (anim);
  }

  Md3Anim_t     GetAnimation2(e_AAnimationAnimType Type)
  {
    Md3PlayerAnim_t      Md3PlayerAnim;

    switch (Type)
      {
      case AA_Death :   Md3PlayerAnim = m_Animation.GetAnim(kBothDeath1); break;
      case AA_Dead :    Md3PlayerAnim = m_Animation.GetAnim(kBothDead1); break;

      case AA_Gesture : Md3PlayerAnim = m_Animation.GetAnim(kLegsIdle); break;

      case AA_Attack :  Md3PlayerAnim = m_Animation.GetAnim(kLegsIdle); break;

      case AA_Stand :   Md3PlayerAnim = m_Animation.GetAnim(kLegsIdle); break;

      case AA_Walk :    Md3PlayerAnim = m_Animation.GetAnim(kLegsWalk); break;
      case AA_Run :     Md3PlayerAnim = m_Animation.GetAnim(kLegsRun); break;
      case AA_Turn :    Md3PlayerAnim = m_Animation.GetAnim(kLegsTurn); break;

      default :         Md3PlayerAnim = m_Animation.GetAnim(kLegsIdle);
      }

    AAnimation::Md3Anim_t       anim;
    anim.first = Md3PlayerAnim.first;
    anim.num = Md3PlayerAnim.num;
    anim.looping = Md3PlayerAnim.looping;
    anim.fps = Md3PlayerAnim.fps;

    return (anim);
  }

  virtual void  SetLOD(RessourceManager::e_RscManagerLOD LOD)
  {
    switch (LOD)
      {
      case RessourceManager::LodLow	: m_Animation.SetLOD(kLodLow); break;
      case RessourceManager::LodMedium	: m_Animation.SetLOD(kLodMedium); break;
      case RessourceManager::LodHigh	: m_Animation.SetLOD(kLodHigh); break;
      default : m_Animation.SetLOD(kLodLow);
      }
  }

  virtual void	Animate(float time)
  {
    m_Animation.animate(time);
  }

  virtual void	Draw() const
  {
    if (m_TranfoList.empty() == false)
      {
	glPushMatrix();
	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	     itr != m_TranfoList.end();
	     ++itr)
	  (*itr)->Active();
      }

    m_Animation.draw();

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }

  void  Draw(int curr_frame, int next_frame, float interp,
	     int curr_frame2, int next_frame2, float interp2) const
  {
    if (m_TranfoList.empty() == false)
      {
	glPushMatrix();
	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	     itr != m_TranfoList.end();
	     ++itr)
	  (*itr)->Active();
      }

    m_Animation.draw(curr_frame, next_frame, interp,
		     curr_frame2, next_frame2, interp2);

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }

};



class	AnimationAlien : public AAnimation
{
private :

  Md3Alien	m_Animation;

public :

  AnimationAlien(const std::string &path)
    : m_Animation(path)
  {
  }

  virtual void	SetAnimation(e_AAnimationAnimType Type)
  {
    switch (Type)
      {
      case AA_Death :	m_Animation.setAnimation(kADeath1); break;
      case AA_Dead :	m_Animation.setAnimation(kADead1); break;

      case AA_Gesture :	m_Animation.setAnimation(kAGesture); break;

      case AA_Attack :	m_Animation.setAnimation(kAAttack1); break;

      case AA_Stand :	m_Animation.setAnimation(kAStand); break;

      case AA_Walk :	m_Animation.setAnimation(kAWalk); break;
      case AA_Run :	m_Animation.setAnimation(kARun); break;
      case AA_Turn :	m_Animation.setAnimation(kATurn); break;

      default :		m_Animation.setAnimation(kAStand);
      }
  }

  Md3Anim_t	GetAnimation(e_AAnimationAnimType Type)
  {
    Md3AlienAnim_t	Md3AlienAnim;

    switch (Type)
      {
      case AA_Death :	Md3AlienAnim = m_Animation.GetAnim(kADeath1); break;
      case AA_Dead :	Md3AlienAnim = m_Animation.GetAnim(kADead1); break;

      case AA_Gesture :	Md3AlienAnim = m_Animation.GetAnim(kAGesture); break;

      case AA_Attack :	Md3AlienAnim = m_Animation.GetAnim(kAAttack1); break;

      case AA_Stand :	Md3AlienAnim = m_Animation.GetAnim(kAStand); break;

      case AA_Walk :	Md3AlienAnim = m_Animation.GetAnim(kAWalk); break;
      case AA_Run :	Md3AlienAnim = m_Animation.GetAnim(kARun); break;
      case AA_Turn :	Md3AlienAnim = m_Animation.GetAnim(kATurn); break;

      default :		Md3AlienAnim = m_Animation.GetAnim(kAStand);
      }

    AAnimation::Md3Anim_t	anim;
    anim.first = Md3AlienAnim.first;
    anim.num = Md3AlienAnim.num;
    anim.looping = Md3AlienAnim.looping;
    anim.fps = Md3AlienAnim.fps;

    return (anim);
  }

  virtual void  SetLOD(RessourceManager::e_RscManagerLOD LOD)
  {
    static_cast<void>(LOD);
  }

  virtual void	Animate(float time)
  {
    m_Animation.animate(time);
  }

  virtual void	Draw() const
  {
    if (m_TranfoList.empty() == false)
      {
  	glPushMatrix();
  	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
  	     itr != m_TranfoList.end();
  	     ++itr)
  	  (*itr)->Active();
      }

    m_Animation.draw();

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }

  void  Draw(int curr_frame, int next_frame, float interp) const
  {
    if (m_TranfoList.empty() == false)
      {
	glPushMatrix();
	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	     itr != m_TranfoList.end();
	     ++itr)
	  (*itr)->Active();
      }

    m_Animation.draw(curr_frame, next_frame, interp);

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }

  void  Draw2(int curr_frame, int next_frame, float interp) const
  {
    // if (m_TranfoList.empty() == false)
    //   {
    // 	glPushMatrix();
    // 	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
    // 	     itr != m_TranfoList.end();
    // 	     ++itr)
    // 	  (*itr)->Active();
    //   }

    m_Animation.draw2(curr_frame, next_frame, interp);

    // if (m_TranfoList.empty() == false)
    //   glPopMatrix();
  }

};


struct Md3Anim_t
{
  int first;
  int num;
  int looping;
  int fps;
};


class   SubAlien : public Objet3D
{
private :

  AnimationAlien&	m_AnimationAlien;
  float	m_speed;

  float curr_time;
  float old_time;
  int curr_frame;
  int next_frame;
  float interp;
  Md3Anim_t anim;

public :

  SubAlien(AnimationAlien& AAnimation)
    : m_AnimationAlien(AAnimation)
  {
    m_speed = 1.0f;

    curr_time = 0.0f;
    old_time = 0.0f;
    curr_frame = 0;
    next_frame = 0;
    interp = 0.0f;

    anim.first = m_AnimationAlien.GetAnimation(AAnimation::AA_Stand).first;
    anim.num = m_AnimationAlien.GetAnimation(AAnimation::AA_Stand).num;
    anim.looping = m_AnimationAlien.GetAnimation(AAnimation::AA_Stand).looping;
    anim.fps = m_AnimationAlien.GetAnimation(AAnimation::AA_Stand).fps;
  }

  void	SetSpeed(float speed)
  {
    m_speed = speed;
  }

  void  SetAnimation(AAnimation::e_AAnimationAnimType Type)
  {
    anim.first = m_AnimationAlien.GetAnimation(Type).first;
    anim.num = m_AnimationAlien.GetAnimation(Type).num;
    anim.looping = m_AnimationAlien.GetAnimation(Type).looping;
    anim.fps = m_AnimationAlien.GetAnimation(Type).fps;

    curr_frame = anim.first;
    next_frame = anim.first;
    interp = 0.0f;
  }

  void	Advance(float dt)
  {
    curr_time += dt * m_speed;

    if ((curr_time - old_time) > (1.0 / anim.fps))
      {
	curr_frame = next_frame;
	next_frame++;

	if (next_frame > (anim.first + anim.num - 1))
	  next_frame = anim.first;

	old_time = curr_time;
      }

    interp = anim.fps * (curr_time - old_time);
  }

  void	Draw()
  {
    if (m_TranfoList.empty() == false)
      {
	glPushMatrix();
	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	     itr != m_TranfoList.end();
	     ++itr)
	  (*itr)->Active();
      }

    m_AnimationAlien.Draw(curr_frame, next_frame, interp);

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }


  void	VMatMult(float *M, sf::Vector3f& vec)
  {
    static float	v[4];
    static float	res[4];

    v[0] = vec.x;
    v[1] = vec.y;
    v[2] = vec.z;
    v[3] = 1.0f;

    res[ 0 ] = M[ 0 ] * v[ 0 ] + M[ 4 ] * v[ 1 ] + M[  8 ] * v[ 2 ] + M[ 12 ] * v[ 3 ];
    res[ 1 ] = M[ 1 ] * v[ 0 ] + M[ 5 ] * v[ 1 ] + M[  9 ] * v[ 2 ] + M[ 13 ] * v[ 3 ];
    res[ 2 ] = M[ 2 ] * v[ 0 ] + M[ 6 ] * v[ 1 ] + M[ 10 ] * v[ 2 ] + M[ 14 ] * v[ 3 ];
    res[ 3 ] = M[ 3 ] * v[ 0 ] + M[ 7 ] * v[ 1 ] + M[ 11 ] * v[ 2 ] + M[ 15 ] * v[ 3 ];

    vec.x = res[ 0 ];
    vec.y = res[ 1 ];
    vec.z = res[ 2 ];
  }


  void	Draw2()
  {
    // if (m_TranfoList.empty() == false)
    //   {
    // 	glPushMatrix();
    // 	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
    // 	     itr != m_TranfoList.end();
    // 	     ++itr)
    // 	  (*itr)->Active();
    //   }


    float mat[16];

    {
      glPushMatrix();

      glLoadIdentity();

      for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	   itr != m_TranfoList.end();
	   ++itr)
	(*itr)->Active();

      glGetFloatv(GL_MODELVIEW_MATRIX, mat);

      glPopMatrix();
    }


    m_AnimationAlien.Draw2(curr_frame, next_frame, interp);

    for (unsigned int i = 0; i < Renderer::m_Vertices.size(); ++i)
      VMatMult( mat, Renderer::m_Vertices[i] );

    // if (m_TranfoList.empty() == false)
    //   glPopMatrix();
  }

};




class   SubPlayer : public Objet3D
{
private :

  AnimationPlayer&	m_AnimationPlayer;
  float	m_speed;

  float curr_time;
  float old_time;
  int curr_frame;
  int next_frame;
  float interp;
  Md3Anim_t anim;

  float curr_time2;
  float old_time2;
  int curr_frame2;
  int next_frame2;
  float interp2;
  Md3Anim_t anim2;

public :
  SubPlayer(AnimationPlayer& AAnimation)
    : m_AnimationPlayer(AAnimation)
  {
    m_speed = 1.0f;

    curr_time = 0.0f;
    old_time = 0.0f;
    curr_frame = 0;
    next_frame = 0;
    interp = 0.0f;

    anim.first = m_AnimationPlayer.GetAnimation(AAnimation::AA_Stand).first;
    anim.num = m_AnimationPlayer.GetAnimation(AAnimation::AA_Stand).num;
    anim.looping = m_AnimationPlayer.GetAnimation(AAnimation::AA_Stand).looping;
    anim.fps = m_AnimationPlayer.GetAnimation(AAnimation::AA_Stand).fps;

    curr_time2 = 0.0f;
    old_time2 = 0.0f;
    curr_frame2 = 0;
    next_frame2 = 0;
    interp2 = 0.0f;

    anim2.first = m_AnimationPlayer.GetAnimation2(AAnimation::AA_Stand).first;
    anim2.num = m_AnimationPlayer.GetAnimation2(AAnimation::AA_Stand).num;
    anim2.looping = m_AnimationPlayer.GetAnimation2(AAnimation::AA_Stand).looping;
    anim2.fps = m_AnimationPlayer.GetAnimation2(AAnimation::AA_Stand).fps;
  }

  void	SetSpeed(float speed)
  {
    m_speed = speed;
  }

  void  SetAnimation(AAnimation::e_AAnimationAnimType Type)
  {
    anim.first = m_AnimationPlayer.GetAnimation(Type).first;
    anim.num = m_AnimationPlayer.GetAnimation(Type).num;
    anim.looping = m_AnimationPlayer.GetAnimation(Type).looping;
    anim.fps = m_AnimationPlayer.GetAnimation(Type).fps;

    curr_frame = anim.first;
    next_frame = anim.first;
    interp = 0.0f;

    anim2.first = m_AnimationPlayer.GetAnimation2(Type).first;
    anim2.num = m_AnimationPlayer.GetAnimation2(Type).num;
    anim2.looping = m_AnimationPlayer.GetAnimation2(Type).looping;
    anim2.fps = m_AnimationPlayer.GetAnimation2(Type).fps;

    curr_frame2 = anim2.first;
    next_frame2 = anim2.first;
    interp2 = 0.0f;
  }

  void	Advance(float dt)
  {
    curr_time += dt * m_speed;

    if ((curr_time - old_time) > (1.0 / anim.fps))
      {
	curr_frame = next_frame;
	next_frame++;

	if (next_frame > (anim.first + anim.num - 1))
	  next_frame = anim.first;

	old_time = curr_time;
      }

    interp = anim.fps * (curr_time - old_time);


    curr_time2 += dt * m_speed;

    if ((curr_time2 - old_time2) > (1.0 / anim2.fps))
      {
	curr_frame2 = next_frame2;
	next_frame2++;

	if (next_frame2 > (anim2.first + anim2.num - 1))
	  next_frame2 = anim2.first;

	old_time2 = curr_time2;
      }

    interp2 = anim2.fps * (curr_time2 - old_time2);
  }

  void	Draw()
  {
    if (m_TranfoList.empty() == false)
      {
	glPushMatrix();
	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
	     itr != m_TranfoList.end();
	     ++itr)
	  (*itr)->Active();
      }

    m_AnimationPlayer.Draw(curr_frame, next_frame, interp,
			   curr_frame2, next_frame2, interp2);

    if (m_TranfoList.empty() == false)
      glPopMatrix();
  }

  // void	Draw2()
  // {
  //   // if (m_TranfoList.empty() == false)
  //   //   {
  //   // 	glPushMatrix();
  //   // 	for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
  //   // 	     itr != m_TranfoList.end();
  //   // 	     ++itr)
  //   // 	  (*itr)->Active();
  //   //   }


  //   float mat[16];

  //   {
  //     glPushMatrix();

  //     glLoadIdentity();

  //     for (std::vector<ATransformation*>::const_iterator itr = m_TranfoList.begin();
  // 	   itr != m_TranfoList.end();
  // 	   ++itr)
  // 	(*itr)->Active();

  //     glGetFloatv(GL_MODELVIEW_MATRIX, mat);

  //     glPopMatrix();
  //   }


  //   m_AnimationAlien.Draw2(curr_frame, next_frame, interp);

  //   // m_AnimationPlayer.Draw2(curr_frame, next_frame, interp,
  //   // 			    curr_frame2, next_frame2, interp2);

  //   for (unsigned int i = 0; i < Renderer::m_Vertices.size(); ++i)
  //     VMatMult( mat, Renderer::m_Vertices[i] );

  //   // if (m_TranfoList.empty() == false)
  //   //   glPopMatrix();
  // }

};



#endif
