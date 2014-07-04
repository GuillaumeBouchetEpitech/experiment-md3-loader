//
// AAnimation.cpp for bomberman in /home/bouche_k/bomberman-2015-candel_s/3D
// 
// Made by guillaume bouchet
// Login   <bouche_k@epitech.net>
// 
// Started on  Sun Jun  3 20:13:21 2012 guillaume bouchet
// Last update Sun Jun  3 21:12:41 2012 guillaume bouchet
//

#include "AAnimation.hh"

AAnimation::AAnimation()
{
}

AAnimation::~AAnimation()
{
  ClearTransformations();
}

// void	AAnimation::DrawCS()
// {
//   /**


//   /////////////////////////
//   BeginFrame(Frame);
//   /////////////////////////

//   float TmpShade;
//   MATRIX TmpMatrix;
//   VECTOR TmpVector, TmpNormal;


//   lightAngle.X = 0.0f;
//   lightAngle.Y = 0.0f;
//   lightAngle.Z = 1.0f;

//   Normalize (lightAngle);



//   {
//     glActiveTextureARB(GL_TEXTURE1_ARB);

//     glEnable(GL_TEXTURE_1D);
//     m_pT1D->Bind();


//     glActiveTextureARB(GL_TEXTURE0_ARB);

//     glEnable(GL_TEXTURE_2D);

//     /////////////////////////
//     Bind();
//     /////////////////////////

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     for (int i = 0 ; i < 16; i++)
//       TmpMatrix.Data[i] = 0;

//     TmpMatrix.Data[1] = TmpMatrix.Data[4] = TmpMatrix.Data[10] = 0.25;

//   }




//   glBegin(GL_TRIANGLES);

//   do
//     {

//       for (int i = 0; i < 3; i++)
// 	{
// 	  /////////////////////////
// 	  Vector2f	texture = GetTexture(i);
// 	  Vector3f	normale = GetNormale(i);
// 	  Vector3f	vertex = GetVertex(i);
// 	  /////////////////////////

// 	  {
// 	    TmpNormal.X = normale.x;
// 	    TmpNormal.Y = normale.y;
// 	    TmpNormal.Z = normale.z;
// 	    RotateVector (TmpMatrix, TmpNormal, TmpVector);
// 	    Normalize (TmpVector);
// 	    TmpShade = DotProduct (TmpVector, lightAngle);
// 	    if (TmpShade < 0.0f)
// 	      TmpShade = 0.0f;
// 	    glMultiTexCoord1fARB(GL_TEXTURE1_ARB, TmpShade);
// 	  }

// 	  glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texture.x, texture.y);
// 	  glVertex3f(vertex.x, vertex.y, vertex.z);
// 	}

//     }
//   /////////////////////////
//   while (Advance() == true);
//   /////////////////////////

//   glEnd();

//   /////////////////////////
//   EndFrame();
//   /////////////////////////


//   {
//     glActiveTextureARB(GL_TEXTURE0_ARB);
//     glDisable(GL_TEXTURE_2D);

//     glActiveTextureARB(GL_TEXTURE1_ARB);
//     glDisable(GL_TEXTURE_1D);

//     glActiveTextureARB(GL_TEXTURE0_ARB);
//   }








//   BeginFrame(Frame);

//   {
//     glEnable(GL_CULL_FACE);

//     glDisable(GL_LIGHTING);
//     glColor3ub(0,0,0);

//     {
//       glPolygonMode(GL_FRONT, GL_LINE);
//       glCullFace(GL_BACK);
//     }

//     glDepthFunc (GL_LEQUAL);
//   }


//   glBegin(GL_TRIANGLES);

//   do
//     {

//       for (int i = 0; i < 3; i++)
// 	{
// 	  Vector3f	vertex = GetVertex(i);

// 	  glVertex3f(vertex.x, vertex.y, vertex.z);
// 	}

//     }
//   while (Advance() == true);

//   glEnd();

//   EndFrame();


//   {
//     glDepthFunc(GL_LESS);
//     glCullFace(GL_BACK);

//     {
//       glPolygonMode(GL_FRONT, GL_FILL);
//     }

//     glDisable(GL_CULL_FACE);
//   }

//   //*/
// }


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

