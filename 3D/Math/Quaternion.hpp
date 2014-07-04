

#ifndef __MATHLIB_H__
#define __MATHLIB_H__


#include <cmath>
#include <cassert>


#include "Mathlib.hpp"


template <typename Real>
class Quaternion
{
public:
  Quaternion()
  {
  }

  Quaternion(Real w, Real x, Real y, Real z)
    : _w (w), _x (x), _y (y), _z (z)
  {
  }

public:
  void identity()
  {
    _w = 1.0; _x = _y = _z = 0.0;
  }

  void normalize ()
  {
    Real mag = std::sqrt ((_w * _w) + (_x * _x) + (_y * _y) + (_z * _z));

    if (mag > 0.0)
      {
	Real oneOverMag = 1.0 / mag;

	_w *= oneOverMag;
	_x *= oneOverMag;
	_y *= oneOverMag;
	_z *= oneOverMag;
      }
  }

  void computeW ()
  {
    Real t = 1.0 - (_x * _x) - (_y * _y) - (_z * _z);

    if (t < 0.0)
      _w = 0.0;
    else
      _w = -std::sqrt (t);
  }

  void rotate (Vector3<Real> &v) const
  {
    Quaternion<Real> qf = *this * v * ~(*this);
    v._x = qf._x;
    v._y = qf._y;
    v._z = qf._z;
  }

  void fromMatrix (const Matrix4x4<Real> &m)
  {
    Real trace = m._m11 + m._m22 + m._m33 + 1.0;

    if (trace > 0.0001)
      {
	Real s = 0.5 / std::sqrt (trace);
	_w = 0.25 / s;
	_x = (m._m23 - m._m32) * s;
	_y = (m._m31 - m._m13) * s;
	_z = (m._m12 - m._m21) * s;
      }
    else
      {
	if ((m._m11 > m._m22) && (m._m11 > m._m33))
	  {
	    Real s = 0.5 / std::sqrt (1.0 + m._m11 - m._m22 - m._m33);
	    _x = 0.25 / s;
	    _y = (m._m21 + m._m12) * s;
	    _z = (m._m31 + m._m13) * s;
	    _w = (m._m32 - m._m23) * s;
	  }
	else if (m._m22 > m._m33)
	  {
	    Real s = 0.5 / std::sqrt (1.0 + m._m22 - m._m11 - m._m33);
	    _x = (m._m21 + m._m12) * s;
	    _y = 0.25 / s;
	    _z = (m._m32 + m._m23) * s;
	    _w = (m._m31 - m._m13) * s;
	  }
	else
	  {
	    Real s = 0.5 / std::sqrt (1.0 + m._m33 - m._m11 - m._m22);
	    _x = (m._m31 + m._m13) * s;
	    _y = (m._m32 + m._m23) * s;
	    _z = 0.25 / s;
	    _w = (m._m21 - m._m12) * s;
	  }
      }
  }

  void fromEulerAngles (Real x, Real y, Real z)
  {
    Real sr = std::sin (x * 0.5);
    Real cr = std::cos (x * 0.5);
    Real sp = std::sin (y * 0.5);
    Real cp = std::cos (y * 0.5);
    Real sy = std::sin (z * 0.5);
    Real cy = std::cos (z * 0.5);

    _w =  (cy * cp * cr) + (sy * sp * sr);
    _x = -(sy * sp * cr) + (cy * cp * sr);
    _y =  (cy * sp * cr) + (sy * cp * sr);
    _z = -(cy * sp * sr) + (sy * cp * cr);
  }

  void toEulerAngles (Real &x, Real &y, Real &z) const
  {
    y = std::asin (2.0 * ((_x * _z) + (_w * _y)));

    Real cy = std::cos (y);
    Real oneOverCosY = 1.0 / cy;

    if (std::fabs (cy) > 0.001)
      {
	x = std::atan2 (2.0 * ((_w * _x) - (_y * _z)) * oneOverCosY,
			(1.0 - 2.0 * (_x*_x + _y*_y)) * oneOverCosY);
	z = std::atan2 (2.0 * ((_w * _z) - (_x * _y)) * oneOverCosY,
			(1.0 - 2.0 * (_y*_y + _z*_z)) * oneOverCosY);
      }
    else
      {
	x = 0.0;
	z = std::atan2 (2.0 * ((_x * _y) + (_w * _z)),
			1.0 - 2.0 * (_x*_x + _z*_z));
      }
  }

  Real rotationAngle () const
  {
    Real thetaOver2 = safeAcos (_w);

    return thetaOver2 * 2.0;
  }

  Vector3<Real> rotationAxis () const
  {
    Real sinThetaOver2Sq = 1.0 - (_w * _w);

    if (sinThetaOver2Sq <= 0.0)
      return Vector3<Real> (1.0, 0.0, 0.0);

    Real oneOverSinThetaOver2 = 1.0 / std::sqrt (sinThetaOver2Sq);

    return Vector3<Real> (_x * oneOverSinThetaOver2,
			  _y * oneOverSinThetaOver2,
			  _z * oneOverSinThetaOver2);
  }

  Quaternion<Real> operator+ (const Quaternion<Real> &q) const
  {
    return Quaternion<Real> (_w + q._w, _x + q._x, _y + q._y, _z + q._z);
  }

  Quaternion<Real> &operator+= (const Quaternion<Real> &q)
  {
    _w += q._w; _x += q._x; _y += q._y; _z += q._z;
    return *this;
  }

  Quaternion<Real> operator- (const Quaternion<Real> &q) const
  {
    return Quaternion<Real> (_w - q._w, _x - q._x, _y - q._y, _z - q._z);
  }

  Quaternion<Real> &operator-= (const Quaternion<Real> &q)
  {
    _w -= q._w; _x -= q._x; _y -= q._y; _z -= q._z;
    return *this;
  }

  Quaternion<Real> operator* (const Quaternion<Real> &q) const
  {
    return Quaternion<Real> ((_w * q._w) - (_x * q._x) - (_y * q._y) - (_z * q._z),
			     (_x * q._w) + (_w * q._x) + (_y * q._z) - (_z * q._y),
			     (_y * q._w) + (_w * q._y) + (_z * q._x) - (_x * q._z),
			     (_z * q._w) + (_w * q._z) + (_x * q._y) - (_y * q._x));
  }

  Quaternion<Real> &operator*= (const Quaternion<Real> &q)
  {
    *this = *this * q;
    return *this;
  }

  Quaternion<Real> operator* (Real k) const
  {
    return Quaternion<Real> (_w * k, _x * k, _y * k, _z * k);
  }

  Quaternion<Real> &operator*= (Real k)
  {
    _w *= k; _x *= k; _y *= k; _z *= k;
    return *this;
  }

  Quaternion<Real> operator* (const Vector3<Real> &v) const
  {
    return Quaternion<Real> (- (_x * v._x) - (_y * v._y) - (_z * v._z),
			     (_w * v._x) + (_y * v._z) - (_z * v._y),
			     (_w * v._y) + (_z * v._x) - (_x * v._z),
			     (_w * v._z) + (_x * v._y) - (_y * v._x));
  }

  Quaternion<Real> &operator*= (const Vector3<Real> &v)
  {
    *this = *this * v;
    return *this;
  }

  Quaternion<Real> operator/ (Real k) const
  {
    Real oneOverK = 1.0 / k;
    return Quaternion<Real> (_w * oneOverK,
			     _x * oneOverK,
			     _y * oneOverK,
			     _z * oneOverK);
  }

  Quaternion<Real> &operator/= (Real k)
  {
    Real oneOverK = 1.0 / k;
    _w *= oneOverK; _x *= oneOverK; _y *= oneOverK; _z *= oneOverK;
    return *this;
  }

  Quaternion<Real> operator~ () const
  {
    return Quaternion<Real> (_w, -_x, -_y, -_z);
  }

  Quaternion<Real> operator- () const
  {
    return Quaternion<Real> (-_w, -_x, -_y, -_z);
  }

public:

  union
  {
    struct
    {
      Real _w, _x, _y, _z;
    };

    Real _q[4];
  };

};


typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;

static const Quaternionf kQuaternionIdentityf (1.0f, 0.0f, 0.0f, 0.0f);
static const Quaterniond kQuaternionIdentityd (1.0f, 0.0f, 0.0f, 0.0f);


template <typename Real>
Quaternion<Real> operator* (Real k, const Quaternion<Real> &q)
{
  return Quaternion<Real> (q._w * k, q._x * k, q._y * k, q._z * k);
}


template <typename Real>
Real DotProduct (const Quaternion<Real> &a, const Quaternion<Real> &b)
{
  return ((a._w * b._w) + (a._x * b._x) + (a._y * b._y) + (a._z * b._z));
}


template <typename Real>
Quaternion<Real> Conjugate (const Quaternion<Real> &q)
{
  return Quaternion<Real> (q._w, -q._x, -q._y, -q._z);
}


template <typename Real>
Quaternion<Real> Inverse (const Quaternion<Real> &q)
{
  Quaternion<Real> res (q._w, -q._x, -q._y, -q._z);
  res.normalize ();
  return res;
}


template <typename Real>
Quaternion<Real> RotationQuaternion (Axis axis, Real theta)
{
  Quaternion<Real> res;

  Real thetaOver2 = theta * 0.5;

  switch (axis)
    {
    case kXaxis:
      res._w = std::cos (thetaOver2);
      res._x = std::sin (thetaOver2);
      res._y = 0.0;
      res._z = 0.0;
      break;

    case kYaxis:
      res._w = std::cos (thetaOver2);
      res._x = 0.0;
      res._y = std::sin (thetaOver2);
      res._z = 0.0;
      break;

    case kZaxis:
      res._w = std::cos (thetaOver2);
      res._x = 0.0;
      res._y = 0.0;
      res._z = std::sin (thetaOver2);
      break;

    default:
      assert (false);
  }

  return res;
}


template <typename Real>
Quaternion<Real> RotationQuaternion (const Vector3<Real> &axis, Real theta)
{
  Quaternion<Real> res;

  assert (std::fabs (DotProduct (axis, axis) - 1.0) < 0.001);

  Real thetaOver2 = theta * 0.5;
  Real sinThetaOver2 = std::sin (thetaOver2);

  res._w = std::cos (thetaOver2);
  res._x = axis._x * sinThetaOver2;
  res._y = axis._y * sinThetaOver2;
  res._z = axis._z * sinThetaOver2;

  return res;
}


template <typename Real>
Quaternion<Real> Log (const Quaternion<Real> &q)
{
  Quaternion<Real> res;
  res._w = 0.0;

  if (std::fabs (q._w) < 1.0)
    {
      Real theta = std::acos (q._w);
      Real sin_theta = std::sin (theta);

      if (std::fabs (sin_theta) > 0.00001)
	{
	  Real thetaOverSinTheta = theta / sin_theta;
	  res._x = q._x * thetaOverSinTheta;
	  res._y = q._y * thetaOverSinTheta;
	  res._z = q._z * thetaOverSinTheta;
	  return res;
	}
    }

  res._x = q._x;
  res._y = q._y;
  res._z = q._z;

  return res;
}


template <typename Real>
Quaternion<Real> Exp (const Quaternion<Real> &q)
{
  Real theta = std::sqrt (DotProduct (q, q));
  Real sin_theta = std::sin (theta);

  Quaternion<Real> res;
  res._w = std::cos (theta);

  if (std::fabs (sin_theta) > 0.00001)
    {
      Real sinThetaOverTheta = sin_theta / theta;
      res._x = q._x * sinThetaOverTheta;
      res._y = q._y * sinThetaOverTheta;
      res._z = q._z * sinThetaOverTheta;
    }
  else
    {
      res._x = q._x;
      res._y = q._y;
      res._z = q._z;
    }

  return res;
}


template <typename Real>
Quaternion<Real> Pow (const Quaternion<Real> &q, Real exponent)
{
  if (std::fabs (q._w) > 0.9999)
    return q;

  Real alpha = std::acos (q._w);

  Real newAlpha = alpha * exponent;

  Vector3<Real> n (q._x, q._y, q._z);
  n *= std::sin (newAlpha) / std::sin (alpha);

  return Quaternion<Real> (std::cos (newAlpha), n);
}


template <typename Real>
Quaternion<Real> Slerp (const Quaternion<Real> &q0, const Quaternion<Real> &q1, Real t)
{
  if (t <= 0.0) return q0;
  if (t >= 1.0) return q1;

  Real cosOmega = DotProduct (q0, q1);

  Real q1w = q1._w;
  Real q1x = q1._x;
  Real q1y = q1._y;
  Real q1z = q1._z;

  if (cosOmega < 0.0)
    {
      q1w = -q1w;
      q1x = -q1x;
      q1y = -q1y;
      q1z = -q1z;
      cosOmega = -cosOmega;
    }

  assert (cosOmega < 1.1);

  Real k0, k1;

  if (cosOmega > 0.9999)
    {
      k0 = 1.0 - t;
      k1 = t;
    }
  else
    {
      Real sinOmega = std::sqrt (1.0 - (cosOmega * cosOmega));

      Real omega = std::atan2 (sinOmega, cosOmega);

      Real oneOverSinOmega = 1.0 / sinOmega;

      k0 = std::sin ((1.0 - t) * omega) * oneOverSinOmega;
      k1 = std::sin (t * omega) * oneOverSinOmega;
    }

  return Quaternion<Real> ((k0 * q0._w) + (k1 * q1w),
			   (k0 * q0._x) + (k1 * q1x),
			   (k0 * q0._y) + (k1 * q1y),
			   (k0 * q0._z) + (k1 * q1z));
}


template <typename Real>
Quaternion<Real> Squad (const Quaternion<Real> &q0,
			const Quaternion<Real> &qa,
			const Quaternion<Real> &qb,
			const Quaternion<Real> &q1,
			Real t)
{
  Real slerp_t = 2.0 * t * (1.0 - t);

  Quaternion<Real> slerp_q0 = Slerp (q0, q1, t);
  Quaternion<Real> slerp_q1 = Slerp (qa, qb, t);

  return Slerp (slerp_q0, slerp_q1, slerp_t);
}


template <typename Real>
inline void Intermediate (const Quaternion<Real> &qprev,
			  const Quaternion<Real> &qcurr,
			  const Quaternion<Real> &qnext,
			  Quaternion<Real> &qa,
			  Quaternion<Real> &qb)
{
  assert (DotProduct (qprev, qprev) <= 1.0001);
  assert (DotProduct (qcurr, qcurr) <= 1.0001);

  Quaternion<Real> inv_prev = Conjugate (qprev);
  Quaternion<Real> inv_curr = Conjugate (qcurr);

  Quaternion<Real> p0 = inv_prev * qcurr;
  Quaternion<Real> p1 = inv_curr * qnext;

  Quaternion<Real> arg = (Log (p0) - Log (p1)) * 0.25;

  qa = qcurr * Exp ( arg);
  qb = qcurr * Exp (-arg);
}


#endif
