

#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP


#include <cmath>
#include <cassert>


#include "Mathlib.hpp"


template <typename Real>
class Matrix4x4
{
public:
  Matrix4x4 ()
    : _h14 (0.0f), _h24 (0.0f), _h34 (0.0f), _tw (1.0f)
  {
  }

public:
  void identity ()
  {
    _m11 = 1.0; _m21 = 0.0; _m31 = 0.0; _tx = 0.0;
    _m12 = 0.0; _m22 = 1.0; _m32 = 0.0; _ty = 0.0;
    _m13 = 0.0; _m23 = 0.0; _m33 = 1.0; _tz = 0.0;
    _h14 = 0.0; _h24 = 0.0; _h34 = 0.0; _tw = 1.0;
  }

  void transpose ()
  {
    *this = Transpose (*this);
  }

  void invert ()
  {
    *this = Invert (*this);
  }

  void setTranslation (const Vector3<Real> &v)
  {
    _tx = v._x; _ty = v._y; _tz = v._z;
  }

  void transform (Vector3<Real> &v) const
  {
    v = Vector3<Real> ((v._x * _m11) + (v._y * _m21) + (v._z * _m31) + _tx,
		       (v._x * _m12) + (v._y * _m22) + (v._z * _m32) + _ty,
		       (v._x * _m13) + (v._y * _m23) + (v._z * _m33) + _tz);
  }

  void rotate (Vector3<Real> &v) const
  {
    v = Vector3<Real> ((v._x * _m11) + (v._y * _m21) + (v._z * _m31),
		       (v._x * _m12) + (v._y * _m22) + (v._z * _m32),
		       (v._x * _m13) + (v._y * _m23) + (v._z * _m33));
  }

  void inverseRotate (Vector3<Real> &v) const
  {
    v = Vector3<Real> ((v._x * _m11) + (v._y * _m12) + (v._z * _m13),
		       (v._x * _m21) + (v._y * _m22) + (v._z * _m23),
		       (v._x * _m31) + (v._y * _m32) + (v._z * _m33));
  }

  void inverseTranslate (Vector3<Real> &v) const
  {
    v._x -= _tx;
    v._y -= _ty;
    v._z -= _tz;
  }

  void fromQuaternion (const Quaternion<Real> &q)
  {
    Real ww = 2.0 * q._w;
    Real xx = 2.0 * q._x;
    Real yy = 2.0 * q._y;
    Real zz = 2.0 * q._z;

    _m11 = 1.0 - (yy * q._y) - (zz * q._z);
    _m12 = (xx * q._y) + (ww * q._z);
    _m13 = (xx * q._z) - (ww * q._y);

    _m21 = (xx * q._y) - (ww * q._z);
    _m22 = 1.0 - (xx * q._x) - (zz * q._z);
    _m23 = (yy * q._z) + (ww * q._x);

    _m31 = (xx * q._z) + (ww * q._y);
    _m32 = (yy * q._z) - (ww * q._x);
    _m33 = 1.0 - (xx * q._x) - (yy * q._y);

    _tx = _ty = _tz = 0.0;
  }

  void fromEulerAngles (Real x, Real y, Real z)
  {
    Real cx = std::cos (x);
    Real sx = std::sin (x);
    Real cy = std::cos (y);
    Real sy = std::sin (y);
    Real cz = std::cos (z);
    Real sz = std::sin (z);

    Real sxsy = sx * sy;
    Real cxsy = cx * sy;

    _m11 =  (cy * cz);
    _m12 =  (sxsy * cz) + (cx * sz);
    _m13 = -(cxsy * cz) + (sx * sz);

    _m21 = -(cy * sz);
    _m22 = -(sxsy * sz) + (cx * cz);
    _m23 =  (cxsy * sz) + (sx * cz);

    _m31 =  (sy);
    _m32 = -(sx * cy);
    _m33 =  (cx * cy);

    _tx = _ty = _tz = 0.0;
  }

  void toEulerAngles (Real &x, Real &y, Real &z) const
  {
    y = std::asin (_m31);

    Real cy = std::cos (y);
    Real oneOverCosY = 1.0 / cy;

    if (std::fabs (cy) > 0.001)
      {
	x = std::atan2 (-_m32 * oneOverCosY, _m33 * oneOverCosY);
	z = std::atan2 (-_m21 * oneOverCosY, _m11 * oneOverCosY);
      }
    else
      {
	x = 0.0;
	z = std::atan2 (_m12, _m22);
      }
  }

  Vector3<Real> rightVector () const
  {
    return Vector3<Real> (_m11, _m12, _m13);
  }

  Vector3<Real> upVector () const
  {
    return Vector3<Real> (_m21, _m22, _m23);
  }

  Vector3<Real> forwardVector () const
  {
    return Vector3<Real> (_m31, _m32, _m33);
  }

  Vector3<Real> translationVector () const
  {
    return Vector3<Real> (_tx, _ty, _tz);
  }

  operator const Real *()
  {
    return _m;
  }

public:

  union
  {
    struct
    {
      Real _m11, _m12, _m13, _h14;
      Real _m21, _m22, _m23, _h24;
      Real _m31, _m32, _m33, _h34;
      Real _tx,  _ty,  _tz,  _tw;
    };

    Real _m[16];
  };

};


typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;


template <typename Real>
Matrix4x4<Real> operator* (const Matrix4x4<Real> &a, const Matrix4x4<Real> &b)
{
  Matrix4x4<Real> res;

  res._m11 = (a._m11 * b._m11) + (a._m21 * b._m12) + (a._m31 * b._m13);
  res._m12 = (a._m12 * b._m11) + (a._m22 * b._m12) + (a._m32 * b._m13);
  res._m13 = (a._m13 * b._m11) + (a._m23 * b._m12) + (a._m33 * b._m13);

  res._m21 = (a._m11 * b._m21) + (a._m21 * b._m22) + (a._m31 * b._m23);
  res._m22 = (a._m12 * b._m21) + (a._m22 * b._m22) + (a._m32 * b._m23);
  res._m23 = (a._m13 * b._m21) + (a._m23 * b._m22) + (a._m33 * b._m23);

  res._m31 = (a._m11 * b._m31) + (a._m21 * b._m32) + (a._m31 * b._m33);
  res._m32 = (a._m12 * b._m31) + (a._m22 * b._m32) + (a._m32 * b._m33);
  res._m33 = (a._m13 * b._m31) + (a._m23 * b._m32) + (a._m33 * b._m33);


  res._tx = (a._m11 * b._tx) + (a._m21 * b._ty) + (a._m31 * b._tz) + a._tx;
  res._ty = (a._m12 * b._tx) + (a._m22 * b._ty) + (a._m32 * b._tz) + a._ty;
  res._tz = (a._m13 * b._tx) + (a._m23 * b._ty) + (a._m33 * b._tz) + a._tz;

  return res;
}

template <typename Real>
Matrix4x4<Real> &operator*= (Matrix4x4<Real> &a, const Matrix4x4<Real> &b)
{
  a = a * b;
  return a;
}

template <typename Real>
Vector3<Real> operator* (const Matrix4x4<Real> &m, const Vector3<Real> &p)
{
  Vector3<Real> res (p);
  m.transform (res);
  return res;
}

template <typename Real>
Matrix4x4<Real> Transpose (const Matrix4x4<Real> &m)
{
  Matrix4x4<Real> res;

  res._m11 = m._m11; res._m21 = m._m12; res._m31 = m._m13; res._tx = m._h14;
  res._m12 = m._m21; res._m22 = m._m22; res._m32 = m._m23; res._ty = m._h24;
  res._m13 = m._m31; res._m23 = m._m32; res._m33 = m._m33; res._tz = m._h34;
  res._h14 = m._tx;  res._h24 = m._ty;  res._h34 = m._tz;  res._tw = m._tw;

  return res;
}

template <typename Real>
inline static Real	Determinant3x3 (const Matrix4x4<Real> &m)
{
  return m._m11 * ((m._m22 * m._m33) - (m._m23 * m._m32))
    + m._m12 * ((m._m23 * m._m31) - (m._m21 * m._m33))
    + m._m13 * ((m._m21 * m._m32) - (m._m22 * m._m31));
}


template <typename Real>
Matrix4x4<Real> Invert (const Matrix4x4<Real> &m)
{
  Real det = Determinant3x3 (m);

  assert (std::fabs (det) > 0.000001);

  Real oneOverDet = 1.0 / det;

  Matrix4x4<Real> res;

  res._m11 = ((m._m22 * m._m33) - (m._m23 * m._m32)) * oneOverDet;
  res._m12 = ((m._m13 * m._m32) - (m._m12 * m._m33)) * oneOverDet;
  res._m13 = ((m._m12 * m._m23) - (m._m13 * m._m22)) * oneOverDet;

  res._m21 = ((m._m23 * m._m31) - (m._m21 * m._m33)) * oneOverDet;
  res._m22 = ((m._m11 * m._m33) - (m._m13 * m._m31)) * oneOverDet;
  res._m23 = ((m._m13 * m._m21) - (m._m11 * m._m23)) * oneOverDet;

  res._m31 = ((m._m21 * m._m32) - (m._m22 * m._m31)) * oneOverDet;
  res._m32 = ((m._m12 * m._m31) - (m._m11 * m._m32)) * oneOverDet;
  res._m33 = ((m._m11 * m._m22) - (m._m12 * m._m21)) * oneOverDet;

  res._tx = -((m._tx * res._m11) + (m._ty * res._m21) + (m._tz * res._m31));
  res._ty = -((m._tx * res._m12) + (m._ty * res._m22) + (m._tz * res._m32));
  res._tz = -((m._tx * res._m13) + (m._ty * res._m23) + (m._tz * res._m33));

  return res;
}


template <typename Real>
Matrix4x4<Real> RotationMatrix (Axis axis, Real theta)
{
  Matrix4x4<Real> res;

  Real s = std::sin (theta);
  Real c = std::cos (theta);

  switch (axis)
    {
    case kXaxis:
      res._m11 = 1.0; res._m21 = 0.0; res._m31 = 0.0;
      res._m12 = 0.0; res._m22 = c;   res._m32 = -s;
      res._m13 = 0.0; res._m23 = s;   res._m33 =  c;
      break;

    case kYaxis:
      res._m11 = c;   res._m21 = 0.0; res._m31 = s;
      res._m12 = 0.0; res._m22 = 1.0; res._m32 = 0.0;
      res._m13 = -s;  res._m23 = 0.0; res._m33 = c;
      break;

    case kZaxis:
      res._m11 = c;   res._m21 = -s;  res._m31 = 0.0;
      res._m12 = s;   res._m22 =  c;  res._m32 = 0.0;
      res._m13 = 0.0; res._m23 = 0.0; res._m33 = 1.0;
      break;

    default:
      assert (false);
    }

  res._tx = res._ty = res._tz = 0.0;

  return res;
}

template <typename Real>
Matrix4x4<Real>	RotationMatrix (const Vector3<Real> &axis, Real theta)
{
  Matrix4x4<Real> res;

  assert (std::fabs (DotProduct (axis, axis) - 1.0) < 0.001);

  Real s = std::sin (theta);
  Real c = std::cos (theta);

  Real a = 1.0 - c;
  Real ax = a * axis._x;
  Real ay = a * axis._y;
  Real az = a * axis._z;

  res._m11 = (ax * axis._x) + c;
  res._m12 = (ax * axis._y) + (axis._z * s);
  res._m13 = (ax * axis._z) - (axis._y * s);

  res._m21 = (ay * axis._x) - (axis._z * s);
  res._m22 = (ay * axis._y) + c;
  res._m23 = (ay * axis._z) + (axis._x * s);

  res._m31 = (az * axis._x) + (axis._y * s);
  res._m32 = (az * axis._y) - (axis._x * s);
  res._m33 = (az * axis._z) + c;

  res._tx = res._ty = res._tz = 0.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> TranslationMatrix (Real x, Real y, Real z)
{
  return TranslationMatrix (Vector3<Real> (x, y, z));
}


template <typename Real>
Matrix4x4<Real> TranslationMatrix (const Vector3<Real> &v)
{
  Matrix4x4<Real> res;

  res._m11 = 1.0; res._m21 = 0.0; res._m31 = 0.0; res._tx = v._x;
  res._m12 = 0.0; res._m22 = 1.0; res._m32 = 0.0; res._ty = v._y;
  res._m13 = 0.0; res._m23 = 0.0; res._m33 = 1.0; res._tz = v._z;

  return res;
}


template <typename Real>
Matrix4x4<Real>	ScaleMatrix (const Vector3<Real> &s)
{
  Matrix4x4<Real> res;

  res._m11 = s._x; res._m21 = 0.0;  res._m31 = 0.0;
  res._m12 = 0.0;  res._m22 = s._y; res._m32 = 0.0;
  res._m13 = 0.0;  res._m23 = 0.0;  res._m33 = s._z;

  res._tx = res._ty = res._tz = 0.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> ScaleAlongAxisMatrix (const Vector3<Real> &axis, Real k)
{
  Matrix4x4<Real> res;

  assert (std::fabs (DotProduct (axis, axis) - 1.0) < 0.001);

  Real a = k - 1.0;
  Real ax = a * axis._x;
  Real ay = a * axis._y;
  Real az = a * axis._z;

  res._m11 = (ax * axis._x) + 1.0;
  res._m22 = (ay * axis._y) + 1.0;
  res._m32 = (az * axis._z) + 1.0;

  res._m12 = res._m21 = (ax * axis._y);
  res._m13 = res._m31 = (ax * axis._z);
  res._m23 = res._m32 = (ay * axis._z);

  res._tx = res._ty = res._tz = 0.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> ShearMatrix (Axis axis, Real s, Real t)
{
  Matrix4x4<Real> res;

  switch (axis)
    {
    case kXaxis:
      res._m11 = 1.0; res._m21 = 0.0; res._m31 = 0.0;
      res._m12 = s;   res._m22 = 1.0; res._m32 = 0.0;
      res._m13 = t;   res._m23 = 0.0; res._m33 = 1.0;
      break;

    case kYaxis:
      res._m11 = 1.0; res._m21 = s;   res._m31 = 0.0;
      res._m12 = 0.0; res._m22 = 1.0; res._m32 = 0.0;
      res._m13 = 0.0; res._m23 = t;   res._m33 = 1.0;
      break;

    case kZaxis:
      res._m11 = 1.0; res._m21 = 0.0; res._m31 = s;
      res._m12 = 0.0; res._m22 = 1.0; res._m32 = t;
      res._m13 = 0.0; res._m23 = 0.0; res._m33 = 1.0;
      break;

    default:
      assert (false);
    }

  res._tx = res._ty = res._tz = 0.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> ProjectionMatrix (const Vector3<Real> &n)
{
  Matrix4x4<Real> res;

  assert (std::fabs (DotProduct (n, n) - 1.0) < 0.001);

  res._m11 = 1.0 - (n._x * n._x);
  res._m22 = 1.0 - (n._y * n._y);
  res._m33 = 1.0 - (n._z * n._z);

  res._m12 = res._m21 = -(n._x * n._y);
  res._m13 = res._m31 = -(n._x * n._z);
  res._m23 = res._m32 = -(n._y * n._z);

  res._tx = res._ty = res._tz = 0.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> ReflectionMatrix (Axis axis, Real k)
{
  Matrix4x4<Real> res;

  switch (axis)
    {
    case kXaxis:
      res._m11 = -1.0; res._m21 =  0.0; res._m31 =  0.0; res._tx = 2.0 * k;
      res._m12 =  0.0; res._m22 =  1.0; res._m32 =  0.0; res._ty = 0.0;
      res._m13 =  0.0; res._m23 =  0.0; res._m33 =  1.0; res._tz = 0.0;
      break;

    case kYaxis:
      res._m11 =  1.0; res._m21 =  0.0; res._m31 =  0.0; res._tx = 0.0;
      res._m12 =  0.0; res._m22 = -1.0; res._m32 =  0.0; res._ty = 2.0 * k;
      res._m13 =  0.0; res._m23 =  0.0; res._m33 =  1.0; res._tz = 0.0;
      break;

    case kZaxis:
      res._m11 =  1.0; res._m21 =  0.0; res._m31 =  0.0; res._tx = 0.0;
      res._m12 =  0.0; res._m22 =  1.0; res._m32 =  0.0; res._ty = 0.0;
      res._m13 =  0.0; res._m23 =  0.0; res._m33 = -1.0; res._tz = 2.0 * k;
      break;

    default:
      assert (false);
    }

  return res;
}


template <typename Real>
Matrix4x4<Real> AxisReflectionMatrix (const Vector3<Real> &n)
{
  Matrix4x4<Real> res;

  assert (std::fabs (DotProduct (n, n) - 1.0) < 0.001);

  Real ax = -2.0 * n._x;
  Real ay = -2.0 * n._y;
  Real az = -2.0 * n._z;

  res._m11 = 1.0 + (ax * n._x);
  res._m22 = 1.0 + (ay * n._y);
  res._m32 = 1.0 + (az * n._z);

  res._m12 = res._m21 = (ax * n._y);
  res._m13 = res._m31 = (ax * n._z);
  res._m23 = res._m32 = (ay * n._z);

  res._tx = res._ty = res._tz = 0.00;

  return res;
}


template <typename Real>
Matrix4x4<Real> LookAtMatrix (const Vector3<Real> &camPos,
			      const Vector3<Real> &target,
			      const Vector3<Real> &camUp)
{
  Matrix4x4<Real> rot, trans;

  Vector3<Real> forward (camPos - target);
  forward.normalize ();

  Vector3<Real> right (CrossProduct (camUp, forward));
  Vector3<Real> up (CrossProduct (forward, right));

  right.normalize ();
  up.normalize ();

  rot._m11 = right._x;
  rot._m21 = right._y;
  rot._m31 = right._z;

  rot._m12 = up._x;
  rot._m22 = up._y;
  rot._m32 = up._z;

  rot._m13 = forward._x;
  rot._m23 = forward._y;
  rot._m33 = forward._z;

  rot._tx  = 0.0;
  rot._ty  = 0.0;
  rot._tz  = 0.0;

  trans = TranslationMatrix (-camPos);

  return (rot * trans);
}


template <typename Real>
Matrix4x4<Real> FrustumMatrix (Real l, Real r,
			       Real b, Real t,
			       Real n, Real f)
{
  assert (n >= 0.0);
  assert (f >= 0.0);

  Matrix4x4<Real> res;

  Real width  = r - l;
  Real height = t - b;
  Real depth  = f - n;

  res._m[0] = (2 * n) / width;
  res._m[1] = 0.0;
  res._m[2] = 0.0;
  res._m[3] = 0.0;

  res._m[4] = 0.0;
  res._m[5] = (2 * n) / height;
  res._m[6] = 0.0;
  res._m[7] = 0.0;

  res._m[8] = (r + l) / width;
  res._m[9] = (t + b) / height;
  res._m[10]= -(f + n) / depth;
  res._m[11]= -1.0;

  res._m[12]= 0.0;
  res._m[13]= 0.0;
  res._m[14]= -(2 * f * n) / depth;
  res._m[15]= 0.0;

  return res;
}

template <typename Real>
Matrix4x4<Real> PerspectiveMatrix (Real fovY, Real aspect, Real n, Real f)
{
  Matrix4x4<Real> res;

  Real angle;
  Real cot;

  angle = fovY / 2.0;
  angle = degToRad (angle);

  cot = std::cos (angle) / std::sin (angle);

  res._m[0] = cot / aspect;
  res._m[1] = 0.0;
  res._m[2] = 0.0;
  res._m[3] = 0.0;

  res._m[4] = 0.0;
  res._m[5] = cot;
  res._m[6] = 0.0;
  res._m[7] = 0.0;

  res._m[8] = 0.0;
  res._m[9] = 0.0;
  res._m[10]= -(f + n) / (f - n);
  res._m[11]= -1.0;

  res._m[12]= 0.0;
  res._m[13]= 0.0;
  res._m[14]= -(2 * f * n) / (f - n);
  res._m[15]= 0.0;

  return res;
}

template <typename Real>
Matrix4x4<Real> OrthoMatrix (Real l, Real r, Real b, Real t, Real n, Real f)
{
  Matrix4x4<Real> res;

  Real width  = r - l;
  Real height = t - b;
  Real depth  = f - n;

  res._m[0] =  2.0 / width;
  res._m[1] =  0.0;
  res._m[2] =  0.0;
  res._m[3] =  0.0;

  res._m[4] =  0.0;
  res._m[5] =  2.0 / height;
  res._m[6] =  0.0;
  res._m[7] =  0.0;

  res._m[8] =  0.0;
  res._m[9] =  0.0;
  res._m[10]= -2.0 / depth;
  res._m[11]=  0.0;

  res._m[12]= -(r + l) / width;
  res._m[13]= -(t + b) / height;
  res._m[14]= -(f + n) / depth;
  res._m[15]=  1.0;

  return res;
}


template <typename Real>
Matrix4x4<Real> OrthoNormalMatrix (const Vector3<Real> &xdir,
	const Vector3<Real> &ydir, const Vector3<Real> &zdir)
{
  Matrix4x4<Real> res;

  res._m[0] = xdir._x; res._m[4] = ydir._x; res._m[8] = zdir._x; res._m[12] = 0.0;
  res._m[1] = xdir._y; res._m[5] = ydir._y; res._m[9] = zdir._y; res._m[13] = 0.0;
  res._m[2] = xdir._z; res._m[6] = ydir._z; res._m[10]= zdir._z; res._m[14] = 0.0;
  res._m[3] = 0.0;     res._m[7] = 0.0;     res._m[11]= 0.0;     res._m[15] = 1.0;

  return res;
}


#endif
