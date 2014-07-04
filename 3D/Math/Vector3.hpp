

#ifndef __VECTOR3_H__
#define __VECTOR3_H__


#include <cmath>
#include <cassert>


#include "Mathlib.hpp"


template <typename Real>
class Vector3
{
public:
  Vector3()
  {
  }

  Vector3(Real x, Real y, Real z)
    : _x (x), _y (y), _z (z)
  {
  }

public:
  bool isZero ()
  {
    return (_x == 0.0) && (_y == 0.0) && (_z == 0.0);
  }

  void normalize ()
  {
    Real magSq = (_x * _x) + (_y * _y) + (_z * _z);

    if (magSq > 0.0)
      {
	Real oneOverMag = 1.0 / std::sqrt (magSq);
	_x *= oneOverMag;
	_y *= oneOverMag;
	_z *= oneOverMag;
      }
  }


public:
  bool operator== (const Vector3<Real> &v) const
  {
    return ((_x == v._x) && (_y == v._y) && (_z == v._z));
  }

  bool operator!= (const Vector3<Real> &v) const
  {
    return ((_x != v._x) || (_y != v._y) || (_z != v._z));
  }

  Vector3<Real> operator- () const
  {
    return Vector3<Real> (-_x, -_y, -_z);
  }

  Vector3<Real> operator+ (const Vector3<Real> &v) const
  {
    return Vector3<Real> (_x + v._x, _y + v._y, _z + v._z);
  }

  Vector3<Real> operator- (const Vector3<Real> &v) const
  {
    return Vector3<Real> (_x - v._x, _y - v._y, _z - v._z);
  }

  Vector3<Real> operator* (Real s) const
  {
    return Vector3<Real> (_x * s, _y * s, _z * s);
  }

  Vector3<Real> operator/ (Real s) const
  {
    Real oneOverS = 1.0 / s;
    return Vector3<Real> (_x * oneOverS, _y * oneOverS, _z * oneOverS);
  }

  Vector3<Real> &operator+= (const Vector3<Real> &v)
  {
    _x += v._x; _y += v._y; _z += v._z;
    return *this;
  }

  Vector3<Real> &operator-= (const Vector3<Real> &v)
  {
    _x -= v._x; _y -= v._y; _z -= v._z;
    return *this;
  }

  Vector3<Real> &operator*= (Real s)
  {
    _x *= s; _y *= s; _z *= s;
    return *this;
  }

  Vector3<Real> &operator/= (Real s)
  {
    Real oneOverS = 1.0 / s;
    _x *= oneOverS; _y *= oneOverS ; _z *= oneOverS;
    return *this;
  }

  operator const Real *()
  {
    return _v;
  }

public:

  union
  {
    struct
    {
      Real _x, _y, _z;
    };

    Real _v[3];
  };

};


typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

static const Vector3f kZeroVector3f (0.0f, 0.0f, 0.0f);
static const Vector3d kZeroVector3d (0.0, 0.0, 0.0);


template <typename Real>
Vector3<Real> operator* (Real k, Vector3<Real> v)
{
  return Vector3<Real> (k * v._x, k * v._y, k * v._z);
}


template <typename Real>
Real VectorMag (const Vector3<Real> &v)
{
  return std::sqrt ((v._x * v._x) +  (v._y * v._y) +  (v._z * v._z));
}


template <typename Real>
Real DotProduct (const Vector3<Real> &a, const Vector3<Real> &b)
{
  return ((a._x * b._x) +  (a._y * b._y) +  (a._z * b._z));
}


template <typename Real>
Vector3<Real> CrossProduct (const Vector3<Real> &a, const Vector3<Real> &b)
{
  return Vector3<Real> ((a._y * b._z) - (a._z * b._y),
			(a._z * b._x) - (a._x * b._z),
			(a._x * b._y) - (a._y * b._x));
}


template <typename Real>
Vector3<Real> ComputeNormal (const Vector3<Real> &p1,
			     const Vector3<Real> &p2,
			     const Vector3<Real> &p3)
{
  Vector3<Real> vec1 (p1 - p2);
  Vector3<Real> vec2 (p1 - p3);

  Vector3<Real> result (CrossProduct (vec1, vec2));
  result.normalize ();

  return result;
}


template <typename Real>
Real Distance (const Vector3<Real> &a, const Vector3<Real> &b)
{
  Real dx = a._x - b._x;
  Real dy = a._y - b._y;
  Real dz = a._z - b._z;
  return std::sqrt ((dx * dx) + (dy * dy) + (dz * dz));
}


template <typename Real>
Real DistanceSquared (const Vector3<Real> &a, const Vector3<Real> &b)
{
  Real dx = a._x - b._x;
  Real dy = a._y - b._y;
  Real dz = a._z - b._z;
  return ((dx * dx) + (dy * dy) + (dz * dz));
}


#endif
