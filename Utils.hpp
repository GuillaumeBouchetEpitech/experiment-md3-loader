

#ifndef UTILS_HPP
#define UTILS_HPP


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cmath>


////////////////////////////////////////////////////////////

float	fast_invsqrtf(float x);

inline float	fast_sqrtf(float x)
{
  return (1.0f / fast_invsqrtf(x));
}

////////////////////////////////////////////////////////////


inline float	length2(float x, float y)
{
  return ( x * x + y * y );
}

inline float	length2(const sf::Vector2f& pos)
{
  return ( length2(pos.x, pos.y) );
}

inline float	length2(float center_x, float center_y, float target_x, float target_y)
{
  return ( length2(target_x - center_x, target_y - center_y) );
}

inline float	length2(const sf::Vector2f& center, const sf::Vector2f& target)
{
  return ( length2(center.x, center.y, target.x, target.y) );
}



inline float	length(float x, float y)
{
  return ( fast_sqrtf( length2(x, y) ) );
}

inline float	length(const sf::Vector2f& pos)
{
  return ( fast_sqrtf( length2(pos) ) );
}

inline float	length(float center_x, float center_y, float target_x, float target_y)
{
  return ( fast_sqrtf( length2(center_x, center_y, target_x, target_y) ) );
}

inline float	length(const sf::Vector2f& center, const sf::Vector2f& target)
{
  return ( length(center.x, center.y, target.x, target.y) );
}


inline void	normalize(sf::Vector2f& v2)
{
  v2 /= length(v2);
}



inline float	getRadAngle(float x, float y)
{
  return (atan2f(y, x));
}

inline float	getRadAngle(const sf::Vector2f& pos)
{
  return ( getRadAngle(pos.x, pos.y) );
}

inline float	getRadAngle(float center_x, float center_y, float target_x, float target_y)
{
  return ( getRadAngle(target_y - center_y, target_x - center_x) );
}

inline float	getRadAngle(const sf::Vector2f& center, const sf::Vector2f& target)
{
  return ( getRadAngle(center.x, center.y, target.x, target.y) );
}



template <typename T>
inline T	rangeRand(T min, T max)
{
  return  (static_cast<T>((min + (static_cast<T>(rand()) / RAND_MAX * (max - min)))));
}

// return ( min + (rand() % (max - min + 1)) );



#endif

