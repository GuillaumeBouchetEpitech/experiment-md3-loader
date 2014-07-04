

#ifndef MD3LODPLAYER_HH
#define MD3LODPLAYER_HH


#include <string>


#include "Md3Player.hh"


class	Md3LodPlayer
{
public :

  Md3LodPlayer(const std::string &path);

  void	SetLOD(Md3PlayerLOD LOD);

  // void setScale (float scale);

  void setAnimation (Md3PlayerAnimType type);

  void setSkin (const std::string &name);

  typedef std::map<std::string, Md3PlayerSkin*> SkinMap;

  const SkinMap &skins () const;


  void	animate(float elapsed_time);

  void	draw() const;
  void  draw (int curr_frame, int next_frame, float interp,
              int curr_frame2, int next_frame2, float interp2) const;


  Md3PlayerAnim_t GetAnim(Md3PlayerAnimType anim) const
  {
    return (m_Hight->GetAnim(anim));
  }


private :

  Md3Player*	m_Hight;
  Md3Player*	m_Medium;
  Md3Player*	m_Low;
  Md3Player*	m_Current;

};




#endif
