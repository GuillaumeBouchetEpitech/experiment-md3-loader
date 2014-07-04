

#include "Md3LodPlayer.hh"


#include <iostream>


Md3LodPlayer::Md3LodPlayer(const std::string &path)
{
  m_Hight = new Md3Player(path, kLodHigh);

  try
    {
      m_Medium = new Md3Player(path, kLodMedium);
    }
  catch (const Md3Exception &exeption)
    {
      m_Medium = NULL;
      // std::cerr << exeption.which() << std::endl;
      std::cerr << exeption.what() << " in " << exeption.where() << std::endl;
    }

  try
    {
      m_Low = new Md3Player(path, kLodLow);
    }
  catch (const Md3Exception &exeption)
    {
      m_Low = NULL;
      // std::cerr << exeption.which() << std::endl;
      std::cerr << exeption.what() << " in " << exeption.where() << std::endl;
    }

  m_Current = m_Hight;
}

void	Md3LodPlayer::SetLOD(Md3PlayerLOD LOD)
{
  if (LOD == kLodHigh)
    m_Current = m_Hight;
  else if (LOD == kLodMedium && m_Medium)
    m_Current = m_Medium;
  else if (LOD == kLodLow && m_Low)
    m_Current = m_Low;
  else if (LOD == kLodLow && m_Medium)
    m_Current = m_Medium;
  // else
  //   std::cerr << "NONE" << std::endl;
}

// void Md3LodPlayer::setScale (float scale)
// {
//   m_Hight->setScale(scale);
//   if (m_Medium)
//     m_Medium->setScale(scale);
//   if (m_Low)
//     m_Low->setScale(scale);
// }

void Md3LodPlayer::setAnimation (Md3PlayerAnimType type)
{
  m_Hight->setAnimation(type);
  if (m_Medium)
    m_Medium->setAnimation(type);
  if (m_Low)
    m_Low->setAnimation(type);
}

void Md3LodPlayer::setSkin (const std::string &name)
{
  m_Hight->setSkin(name);
  if (m_Medium)
    m_Medium->setSkin(name);
  if (m_Low)
    m_Low->setSkin(name);
}

const Md3LodPlayer::SkinMap& Md3LodPlayer::skins () const
{
  return m_Hight->skins();
}

void	Md3LodPlayer::animate(float elapsed_time)
{
  m_Hight->animate(elapsed_time);
  if (m_Medium)
    m_Medium->animate(elapsed_time);
  if (m_Low)
    m_Low->animate(elapsed_time);
}

void	Md3LodPlayer::draw() const
{
  m_Current->draw();
}

// void  Md3LodPlayer::draw(int curr_frame, int next_frame, float interp) const
void  Md3LodPlayer::draw (int curr_frame, int next_frame, float interp,
			  int curr_frame2, int next_frame2, float interp2) const
{
  m_Current->draw(curr_frame, next_frame, interp,
		  curr_frame2, next_frame2, interp2);
}

