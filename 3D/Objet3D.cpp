

#include "AAnimation.hpp"


AAnimation::AAnimation()
{
}

Translation*	Objet3D::AddTranslate(Vector3f Translate)
{
  Translation*	new_Translation = new Translation(Translate);
  m_TranfoList.push_back(new_Translation);
  return (new_Translation);
}

Rotation*	Objet3D::AddRotate(float angle, Vector3f Vector)
{
  Rotation*	new_Rotation = new Rotation(angle, Vector);
  m_TranfoList.push_back(new_Rotation);
  return (new_Rotation);
}

Scale*		Objet3D::AddScale(Vector3f Value)
{
  Scale*	new_Scale = new Scale(Value);
  m_TranfoList.push_back(new_Scale);
  return (new_Scale);
}

void	Objet3D::ClearTransformations()
{
  for (std::vector<ATransformation*>::iterator itr = m_TranfoList.begin();
       itr != m_TranfoList.end();
       ++itr)
    delete *itr;
  m_TranfoList.clear();
}

