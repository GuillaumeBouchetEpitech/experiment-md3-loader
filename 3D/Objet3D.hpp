

#ifndef	OBJECT3D_HPP
#define	OBJECT3D_HPP


#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>



class	ATransformation
{
public :

  ATransformation();

  virtual void	Active() const = 0;

};

//////////////////////////////////////////

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

//////////////////////////////////////////

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

//////////////////////////////////////////

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

//////////////////////////////////////////

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


#endif

