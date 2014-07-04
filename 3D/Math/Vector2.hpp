

#ifndef __VECTOR2_H__
#define __VECTOR2_H__


#include <cmath>
#include <cassert>


#include "Mathlib.hpp"


template <typename Real>
class Vector2
{
public:
  Vector2()
  {
  }

  Vector2(Real x, Real y)
    : _x (x), _y (y)
  {
  }

public:
  bool isZero ()
  {
    return (_x == 0.0) && (_y == 0.0);
  }

  void normalize ()
  {
    Real magSq = (_x * _x) + (_y * _y);

    if (magSq > 0.0)
      {
	Real oneOverMag = 1.0 / std::sqrt (magSq);
	_x *= oneOverMag;
	_y *= oneOverMag;
      }
  }


public:
  bool operator== (const Vector2<Real> &v) const
  {
    return ((_x == v._x) && (_y == v._y));
  }

  bool operator!= (const Vector2<Real> &v) const
  {
    return ((_x != v._x) || (_y != v._y));
  }

  Vector2<Real> operator- () const
  {
    return Vector3<Real> (-_x, -_y);
  }

  Vector2<Real> operator+ (const Vector2<Real> &v) const
  {
    return Vector3<Real> (_x + v._x, _y + v._y);
  }

  Vector2<Real> operator- (const Vector2<Real> &v) const
  {
    return Vector3<Real> (_x - v._x, _y - v._y);
  }

  Vector2<Real> operator* (Real s) const
  {
    return Vector3<Real> (_x * s, _y * s);
  }

  Vector2<Real> operator/ (Real s) const
  {
    Real oneOverS = 1.0 / s;
    return Vector3<Real> (_x * oneOverS, _y * oneOverS);
  }

  Vector2<Real> &operator+= (const Vector2<Real> &v)
  {
    _x += v._x; _y += v._y;
    return *this;
  }

  Vector2<Real> &operator-= (const Vector2<Real> &v)
  {
    _x -= v._x; _y -= v._y;
    return *this;
  }

  Vector2<Real> &operator*= (Real s)
  {
    _x *= s; _y *= s;
    return *this;
  }

  Vector2<Real> &operator/= (Real s)
  {
    Real oneOverS = 1.0 / s;
    _x *= oneOverS; _y *= oneOverS;
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
      Real _x, _y;
    };

    Real _v[3];
  };

};


typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

static const Vector2f kZeroVector2f (0.0f, 0.0f);
static const Vector2d kZeroVector2d (0.0, 0.0);


template <typename Real>
Vector2<Real> operator* (Real k, Vector2<Real> v)
{
  return Vector2<Real> (k * v._x, k * v._y);
}


template <typename Real>
Real VectorMag (const Vector2<Real> &v)
{
  return std::sqrt ((v._x * v._x) +  (v._y * v._y));
}


template <typename Real>
Real DotProduct (const Vector2<Real> &a, const Vector2<Real> &b)
{
  return ((a._x * b._x) +  (a._y * b._y));
}


template <typename Real>
Vector2<Real> CrossProduct (const Vector2<Real> &a, const Vector2<Real> &b)
{
  return Vector2<Real> ((a._y * b._z) - (a._z * b._y),
			(a._z * b._x) - (a._x * b._z));
}


// template <typename Real>
// Vector2<Real> ComputeNormal (const Vector2<Real> &p1,
// 			     const Vector2<Real> &p2)
// {
//   Vector2<Real> vec1 (p1 - p2);
//   Vector2<Real> vec2 (p1 - p3);

//   Vector2<Real> result (CrossProduct (vec1, vec2));
//   result.normalize ();

//   return result;
// }


template <typename Real>
Real Distance (const Vector2<Real> &a, const Vector2<Real> &b)
{
  Real dx = a._x - b._x;
  Real dy = a._y - b._y;
  return std::sqrt ((dx * dx) + (dy * dy));
}


template <typename Real>
Real DistanceSquared (const Vector2<Real> &a, const Vector2<Real> &b)
{
  Real dx = a._x - b._x;
  Real dy = a._y - b._y;
  return ((dx * dx) + (dy * dy));
}


#endif
