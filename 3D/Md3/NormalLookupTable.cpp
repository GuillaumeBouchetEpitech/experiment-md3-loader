
#include "NormalLookupTable.hh"

NormalLookupTable::NormalLookupTable ()
{
  for (int i = 0; i < 256; ++i)
    {
      for (int j = 0; j < 256; ++j)
	{
	  float lng = i * 2.0f * kPi / 255.0f;
	  float lat = j * 2.0f * kPi / 255.0f;

	  _normal[i][j][0] = std::cos (lat) * std::sin (lng);
	  _normal[i][j][1] = std::sin (lat) * std::sin (lng);
	  _normal[i][j][2] = std::cos (lng);
	}
    }
}

const vec3_t*	NormalLookupTable::operator[] (int i) const
{
  return _normal[i];
}

