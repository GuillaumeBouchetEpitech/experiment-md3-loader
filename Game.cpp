

#include "Game.hh"
#include "RuntimeException.hh"
#include "MultiDraw.hpp"

#include "3D/AAnimation.hh"
#include "3D/Renderer.hpp"



#include "GL_Light.hpp"


#include <sstream>





// sf::Vector3f	LightPos(0,20,0);



float LightPos[] = { 0.0f, 5.0f, 5.0f, 0.0f};
// float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};
float LightAmb[] = { 0.05f, 0.05f, 0.05f, 1.0f};
float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};
float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};



Game::Game()
  : m_camera(m_RWindow)
{

  sf::ContextSettings   Settings;

  Settings.depthBits = 24;
  // Settings.stencilBits = 0;
  Settings.stencilBits = 8;
  Settings.antialiasingLevel = 0;

  m_RWindow.create(sf::VideoMode(800, 600, 32), "Md3Loader test",
  		   sf::Style::Close | sf::Style::Resize, Settings);

  // m_RWindow.create(sf::VideoMode(800, 600, 32), "Md3Loader test",
  // 		   sf::Style::Close | sf::Style::Resize);

  m_RWindow.setFramerateLimit(30);
  m_RWindow.setVerticalSyncEnabled(true);


  MultiDraw::Create();


  line_mode = false;
  m_flags = 0;

  aaliasing = false;


  try
    {

      // std::string	base(FILE_PATH);

      // std::cerr << std::endl << base << std::endl << std::endl;

      // std::cerr << std::endl << base + "File/ground.jpg" << std::endl << std::endl;

      RessourceManager::GetInstance()->LoadTexture("File/ground.jpg");
      RessourceManager::GetInstance()->GetTexture("File/ground.jpg")->setRepeated(true);
      RessourceManager::GetInstance()->GetTexture("File/ground.jpg")->setSmooth(true);

      // m_all_Name.push_back("File/Animation/Quake3/anarki");
      // m_all_Name.push_back("File/Animation/Quake3/biker");
      // m_all_Name.push_back("File/Animation/Quake3/bitterman");
      // m_all_Name.push_back("File/Animation/Quake3/bones");
      // m_all_Name.push_back("File/Animation/Quake3/brandon");
      // m_all_Name.push_back("File/Animation/Quake3/carmack");
      // m_all_Name.push_back("File/Animation/Quake3/cash");
      // m_all_Name.push_back("File/Animation/Quake3/crash");
      // m_all_Name.push_back("File/Animation/Quake3/doom");
      // m_all_Name.push_back("File/Animation/Quake3/grunt");
      // m_all_Name.push_back("File/Animation/Quake3/harley");
      // m_all_Name.push_back("File/Animation/Quake3/human_bsuit");
      // m_all_Name.push_back("File/Animation/Quake3/hunter");
      // m_all_Name.push_back("File/Animation/Quake3/keel");
      // m_all_Name.push_back("File/Animation/Quake3/klesk");
      // m_all_Name.push_back("File/Animation/Quake3/lucy");
      // m_all_Name.push_back("File/Animation/Quake3/major");
      // m_all_Name.push_back("File/Animation/Quake3/mynx");
      // m_all_Name.push_back("File/Animation/Quake3/orbb");
      // m_all_Name.push_back("File/Animation/Quake3/paulj");
      // m_all_Name.push_back("File/Animation/Quake3/ranger");
      // m_all_Name.push_back("File/Animation/Quake3/razor");
      // m_all_Name.push_back("File/Animation/Quake3/sarge");
      // m_all_Name.push_back("File/Animation/Quake3/slash");
      // m_all_Name.push_back("File/Animation/Quake3/sorlag");
      // m_all_Name.push_back("File/Animation/Quake3/tankjr");
      // m_all_Name.push_back("File/Animation/Quake3/tim");
      // m_all_Name.push_back("File/Animation/Quake3/uriel");
      // m_all_Name.push_back("File/Animation/Quake3/visor");
      // m_all_Name.push_back("File/Animation/Quake3/xaero");
      // m_all_Name.push_back("File/Animation/Quake3/xian");

      for (std::list<std::string>::iterator itr = m_all_Name.begin();
      	   itr != m_all_Name.end();
      	   ++itr)
      	{
      	  RessourceManager::GetInstance()->LoadPlayer(*itr);

      	  RessourceManager::GetInstance()->GetPlayer(*itr)->AddScale(Vector3f(0.05f, 0.05f, 0.05f));
      	}





      // m_all_Name2.push_back("File/Animation/Tremulous/builder");
      // m_all_Name2.push_back("File/Animation/Tremulous/level0");
      // m_all_Name2.push_back("File/Animation/Tremulous/level1");
      // m_all_Name2.push_back("File/Animation/Unvanquished/builder");
      // m_all_Name2.push_back("File/Animation/Unvanquished/level0");
      // m_all_Name2.push_back("File/Animation/Unvanquished/level1");
      // m_all_Name2.push_back("File/Animation/Unvanquished/level2");
      m_all_Name2.push_back("File/Animation/Unvanquished/level3");

      for (std::list<std::string>::iterator itr = m_all_Name2.begin();
      	   itr != m_all_Name2.end();
      	   ++itr)
      	{
      	  RessourceManager::GetInstance()->LoadAlien(*itr);

      	  // RessourceManager::GetInstance()->GetAlien(*itr)->AddScale(Vector3f(0.05f, 0.05f, 0.05f));
      	}

      int x = 0;

      float	local_z = 1.25f;
      // float	local_x_dec = -3.0f;

      int	dec_y = 0;

      for (std::list<std::string>::iterator itr = m_all_Name2.begin();
      	   itr != m_all_Name2.end();
      	   ++itr)
      	{
	  // int	count = 0;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Death);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Dead);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Gesture);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Attack);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Stand);
	  m_SubAliens.back().SetAnimation(AAnimation::AA_Walk);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));
	  // m_SubAliens.back().AddTranslate( Vector3f(0.0f, 2.0f * dec_y, local_z) );

	  // dec_y += 2;

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Walk);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Run);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++count;

	  // m_SubAliens.push_back( SubAlien( *RessourceManager::GetInstance()->GetAlien(*itr) ) );
	  // m_SubAliens.back().SetAnimation(AAnimation::AA_Turn);
	  // m_SubAliens.back().AddTranslate(Vector3f(x * 2.0f, -2.0f + local_x_dec * count, local_z));

	  // ++x;
	  // ++x;
	}

      x = 0;


      for (std::list<std::string>::iterator itr = m_all_Name.begin();
      	   itr != m_all_Name.end();
      	   ++itr)
	  {
	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Death);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 0.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Dead);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 2.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Gesture);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 4.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Attack);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 6.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Stand);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 8.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Walk);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 10.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Run);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 12.0f, local_z));

	    m_SubPlayers.push_back( SubPlayer( *RessourceManager::GetInstance()->GetPlayer(*itr) ) );
	    m_SubPlayers.back().SetAnimation(AAnimation::AA_Turn);
	    m_SubPlayers.back().AddTranslate(Vector3f(x * 2.0f, 14.0f, local_z));

	    ++x;
	  }

      std::cout << m_all_Name.size() + m_all_Name2.size() << std::endl;


    }
  catch (const RuntimeException& exeption)
    {
      throw RuntimeException(exeption.what(), exeption.where());
    }
  catch (const std::bad_alloc &)
    {
      throw RuntimeException("Empty memory", "Load Model");
    }
  catch (const Md3Exception& exeption)
    {
      throw RuntimeException(exeption.what(), exeption.where());
    }
  catch (...)
    {
      throw RuntimeException("Unknown error", "Load Model");
    }





  this->m_camera.Initialize();


  {
    glClearDepth(1.0f);

    // Stencil Buffer Setup
    glClearStencil(0);

    glEnable(GL_DEPTH_TEST);                              // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                               // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

    glCullFace(GL_BACK);                  // Set Culling Face To Back Face
    glEnable(GL_CULL_FACE);               // Enable Culling

  }

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);



  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.74f, 0.84f, 95.0f, 1.0f);
  glClearDepth(1.0f);

  glEnable (GL_TEXTURE_2D);

  glDisable(GL_CULL_FACE);
  // glEnable(GL_CULL_FACE);




  // GL_Light::Set_Specular(1, 1, 1, 1);
  // GL_Light::Set_Shininess(0.2f);
  // GL_Light::Set_Shininess(50.0f);

  // GL_Light::Set_Diffuse(GL_LIGHT0, 0.4f, 0.4f, 0.4f, 0.4f);


  // float	att = 1000.0f;
  // GL_Light::Set_Constant_Attenuation(GL_LIGHT0, att);
  // GL_Light::Set_Linear_Attenuation(GL_LIGHT0, att);
  // GL_Light::Set_Quadratic_Attenuation(GL_LIGHT0, att);

  // GL_Light::Set_Position(GL_LIGHT0, LightPos.x, LightPos.y, LightPos.z);

  // GL_Light::Enable_Light(GL_LIGHT0);


  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);


  glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);


  float MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};
  float MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};
  float MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};
  float MatShn[] = {0.0f};

  glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);
  glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);


  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


}

Game::~Game()
{

  MultiDraw::Destroy();

}

void	Game::Update(const float elapsedTime)
{
  sf::Event   Event;
  while (this->m_RWindow.pollEvent(Event))
    {
      if (Event.type == sf::Event::Closed ||
	  (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
	this->m_RWindow.close();


#define	D_INVERSE(value)	m_flags = ( (m_flags & (value)) ? (m_flags & ~(value)) : (m_flags |= (value)) )

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F1)
	D_INVERSE(Renderer::eTexture);

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F2)
	D_INVERSE(Renderer::eNormal);

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F3)
	D_INVERSE(Renderer::eCellShading_texture1D);

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F4)
	D_INVERSE(Renderer::eCellShading_color);

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F5)
	D_INVERSE(Renderer::eCellShading_bound);

#undef	D_INVERSE

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F6)
      	line_mode = !line_mode;

      else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F7)
	aaliasing = !aaliasing;


      // else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F1)
      // 	RessourceManager::GetInstance()->SetLOD(RessourceManager::LodLow);
      // else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F2)
      // 	RessourceManager::GetInstance()->SetLOD(RessourceManager::LodMedium);
      // else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F3)
      // 	RessourceManager::GetInstance()->SetLOD(RessourceManager::LodHigh);

      // else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F4)
      // 	line_mode = false;
      // else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F5)
      // 	line_mode = true;

      else
      	this->m_camera.Event(Event);

    }

  this->m_camera.Update(elapsedTime);


  for (std::list<SubAlien>::iterator itr = m_SubAliens.begin();
       itr != m_SubAliens.end();
       ++itr)
    itr->Advance(elapsedTime);

  for (std::list<SubPlayer>::iterator itr = m_SubPlayers.begin();
       itr != m_SubPlayers.end();
       ++itr)
    itr->Advance(elapsedTime);

}

void	Game::Render(void)
{



  static float	angle = 0.0f;
  static float	radius = 10.0f;

  LightPos[0] = radius * cosf( angle );
  LightPos[1] = radius * sinf( angle );
  // LightPos[2] = 3.0f;

  glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

  angle += 0.1f;


  // std::cerr << std::endl;
  // std::cerr << LightPos[0] << std::endl;
  // std::cerr << LightPos[1] << std::endl;




  sf::Clock	Clock;


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  ////
  if (aaliasing)
    glEnable(GL_LINE_SMOOTH);
  else
    glDisable(GL_LINE_SMOOTH);
  ////



  this->m_camera.Look();




  glBegin(GL_LINES);
  {
    glColor3f(1,0,0);

    glVertex3f(0,0,0);
    glVertex3f(1000,0,0);


    glColor3f(0,1,0);

    glVertex3f(0,0,0);
    glVertex3f(0,1000,0);


    glColor3f(0,0,1);

    glVertex3f(0,0,0);
    glVertex3f(0,0,1000);
  }
  glEnd();


  {
    glEnable(GL_TEXTURE_2D);

    sf::Texture::bind(NULL);

    {
      glBegin(GL_LINES);
      glColor3ub(255,255,255);
      glVertex3f(0,0,0);
      glVertex3f(1000,0,0);

      for (float z = -1.0f; z <= 1.0f; z += 0.25f)
	{
	  glVertex3f(0,0,z);
	  glVertex3f(0,1000,z);
	}

      glVertex3f(0,0,0);
      glVertex3f(0,0,1000);
      glEnd();
    }
  }

  {

    // std::string	base(FILE_PATH);
    sf::Texture::bind(RessourceManager::GetInstance()->GetTexture("File/ground.jpg"));

    int	min_x = -2, min_y = -2;
    int	max_x = 2, max_y = 2;

    float	size = 10.0f;

    glColor4f(1,1,1,1);

    glBegin(GL_QUADS);
    {
      glTexCoord2i(min_x, min_y);
      glVertex3f(min_x * size, min_y * size, 0);

      glTexCoord2i(min_x, max_y);
      glVertex3f(min_x * size, max_y * size, 0);

      glTexCoord2i(max_x, max_y);
      glVertex3f(max_x * size, max_y * size, 0);

      glTexCoord2i(max_x, min_y);
      glVertex3f(max_x * size, min_y * size, 0);
    }
    glEnd();
  }



  // GL_Light::Set_Position(GL_LIGHT0, LightPos.x, LightPos.y, LightPos.z);



  glColor3ub(255,255,255);




  for (std::list<SubAlien>::iterator itr = m_SubAliens.begin();
       itr != m_SubAliens.end();
       ++itr)
    {

      glPolygonMode(GL_FRONT_AND_BACK, line_mode ? GL_LINE : GL_FILL);



      glLightfv(GL_LIGHT0, GL_POSITION, LightPos);



      itr->Draw2();



      {
	glEnable(GL_TEXTURE_2D);

	// std::string	base(FILE_PATH);
	sf::Texture::bind(RessourceManager::GetInstance()->GetTexture("File/ground.jpg"));

	int	min_x = -2, min_y = -2;
	int	max_x = 2, max_y = 2;

	float	size = 10.0f;

	glColor4f(1,1,1,1);

	glBegin(GL_QUADS);
	{
	  glTexCoord2i(min_x, min_y);
	  glVertex3f(min_x * size, min_y * size, 0);

	  glTexCoord2i(min_x, max_y);
	  glVertex3f(min_x * size, max_y * size, 0);

	  glTexCoord2i(max_x, max_y);
	  glVertex3f(max_x * size, max_y * size, 0);

	  glTexCoord2i(max_x, min_y);
	  glVertex3f(max_x * size, min_y * size, 0);
	}
	glEnd();
      }


      glDisable(GL_TEXTURE_2D);
      glBindTexture( GL_TEXTURE_2D, 0 );

      GL_Light::Enable_Lighting();
      {
	Renderer::ProtoRender( m_flags );
      }
      GL_Light::Disable_Lighting();

      if (line_mode)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



      glColor3f(1,1,1);
      glBindTexture( GL_TEXTURE_2D, 0 );

      Renderer::CastShadow( sf::Vector3f(LightPos[0], LightPos[1], LightPos[2]) );

      glColor3f(1,1,1);
      glBindTexture( GL_TEXTURE_2D, 0 );



    }

  for (std::list<SubPlayer>::iterator itr = m_SubPlayers.begin();
       itr != m_SubPlayers.end();
       ++itr)
    itr->Draw();



  float	size = 10.0f;

  glColor4f(1,1,1,1);
  glBindTexture( GL_TEXTURE_2D, 0 );

  glBegin(GL_LINES);
  {
    glVertex3f( LightPos[0] - size, LightPos[1], LightPos[2] );
    glVertex3f( LightPos[0] + size, LightPos[1], LightPos[2] );

    glVertex3f( LightPos[0], LightPos[1] - size, LightPos[2] );
    glVertex3f( LightPos[0], LightPos[1] + size, LightPos[2] );

    glVertex3f( LightPos[0], LightPos[1], LightPos[2] - size );
    glVertex3f( LightPos[0], LightPos[1], LightPos[2] + size );
  }
  glEnd();



  glFlush();

  { // 2D

    m_RWindow.pushGLStates();
    m_RWindow.resetGLStates();
    {

      {
	sf::View&	DefaultView = const_cast<sf::View&>(m_RWindow.getDefaultView());
	sf::Vector2f	WSize(m_RWindow.getSize().x, m_RWindow.getSize().y);

	DefaultView.setCenter( WSize.x / 2, WSize.y / 2 );
	DefaultView.setSize( WSize );

	m_RWindow.setView( DefaultView );
      }

      std::stringstream       sstr;

      // sstr << "test1" << std::endl
      // 	   << "test2" << std::endl
      // 	   << "test3";


      sstr << "F1 : Texture (" << (m_flags & Renderer::eTexture ? "ON)" : "OFF)") << std::endl;
      sstr << "F2 : Normal (" << (m_flags & Renderer::eNormal ? "ON)" : "OFF)") << std::endl;
      sstr << "Cell Shading :" << std::endl;
      sstr << " -> F3 : texture 1D (" << (m_flags & Renderer::eCellShading_texture1D ? "ON)" : "OFF)") << std::endl;
      sstr << " -> F4 : color (" << (m_flags & Renderer::eCellShading_color ? "ON)" : "OFF)") << std::endl;
      sstr << " -> F5 : bound (" << (m_flags & Renderer::eCellShading_bound ? "ON)" : "OFF)") << std::endl;
      sstr << "F6 : render mode (" << (line_mode ? "LINE)" : "FILL)") << std::endl;
      sstr << "F7 : anti aliasing (" << (aaliasing ? "ON)" : "OFF)") << std::endl;

      sstr << std::endl << "FPS : " << 1.0f / Clock.getElapsedTime().asSeconds();



      MultiDraw::PrintNow(m_RWindow,
			  sstr.str(),
			  sf::Vector2f(10,10), 20,
			  sf::Color::White, true);



      // sf::Vector2f	min(10, 410);
      // sf::Vector2f	max(290, 590);

      // MultiDraw::Clear();
      // {
      // 	MultiDraw::AddLine(sf::Triangles,
      // 			   min.x, min.y,
      // 			   max.x, max.y,
      // 			   1.0f, sf::Color::Red);
      // }
      // MultiDraw::Render(m_RWindow, sf::Triangles);



    }
    m_RWindow.popGLStates();

  } // 2D

}
