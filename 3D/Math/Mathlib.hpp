

#ifndef MATHLIB_HPP
#define MATHLIB_HPP


#include <cmath>
#include <cassert>


template <typename Real> class Vector3;
template <typename Real> class Matrix4x4;
template <typename Real> class Quaternion;


enum Axis
  {
    kXaxis, kYaxis, kZaxis
  };

const float kPi = 3.14159265358979323846f;
const float k2Pi = kPi * 2.0f;
const float kPiOver2 = kPi / 2.0f;
const float k1OverPi = 1.0f / kPi;
const float k1Over2Pi = 1.0f / k2Pi;
const float kPiOver180 = kPi / 180.0f;
const float k180OverPi = 180.0f / kPi;

template <typename Real>
Real wrapPi (Real theta)
{
  theta += kPi;
  theta -= std::floor (theta * k1Over2Pi) * k2Pi;
  theta -= kPi;
  return theta;
}

template <typename Real>
Real safeAcos (Real x)
{
  if (x <= -1.0)
    return kPi;

  if (x >= 1.0)
    return 0.0;

  return std::acos (x);
}

template <typename Real>
void canonizeEulerAngles (Real &roll, Real &pitch, Real &yaw)
{
  pitch = wrapPi (pitch);

  if (pitch < -kPiOver2)
    {
      roll += kPi;
      pitch = -kPi - pitch;
      yaw += kPi;
    }
  else if (pitch > kPiOver2)
    {
      roll += kPi;
      pitch = kPi - pitch;
      yaw += kPi;
    }

  if (std::fabs (pitch) > kPiOver2 - 1e-4)
    {
      yaw += roll;
      roll = 0.0;
    }
  else
    {
      roll = wrapPi (roll);
    }

  yaw = wrapPi (yaw);
}


template <typename Real>
inline Real degToRad (Real deg)
{
  return deg * kPiOver180;
}

template <typename Real>
inline Real radToDeg (Real rad)
{
  return rad * k180OverPi;
}

template <typename Real>
inline Real fovToZoom (Real fov)
{
  return 1.0f / std::tan (fov * 0.5f);
}

template <typename Real>
inline Real zoomToFov (Real zoom)
{
  return 2.0f * std::atan (1.0f / zoom);
}


#endif

