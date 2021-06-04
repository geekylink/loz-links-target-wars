#include "Crate_State.h"
#include "Title_State.h"

#include <Zeni/Camera.hxx>
#include <Zeni/Collision.hxx>
#include <Zeni/Timer.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Sounds.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <Zeni/Widget.hxx>

using namespace std;
using namespace Zeni;
using namespace Zeni_Collision;

namespace Crate {

static bool ArrowHit = false;
static Zeni::Sound_Source * ArrowShotSound;
static Zeni::Sound_Source * ArrowHitSound;
static Zeni::Sound_Source * ArrowPullSound;
static Target m_targets[TARGETS_IN_USE];

static 	string PlayerName;


  Crate_State::Crate_State()
	  : m_crate(Point3f(100.0f, -15.0f, 0.0f),
				Vector3f(30.0f, 30.0f, 30.0f)),
		m_player(Camera(Point3f(500.0f, 500.0f, GROUND),
						Vector3f(1.0f,0.0f,0.0f),
						Vector3f(0.0f,0.0f,1.0f),
						1.0f, // near clip, as close as it should ever be
						2000.0f, // far clip
						pi / 2.0f), // fovy == field of view, in y
				Vector3f(0.0f, 0.0f, -39.0f), 
				11.0f),
		m_moved(false),// don't replay sound, if you haven't moved
		TBox(Point2f(30.0f, 30.0f), Point2f(100.0f,45.0f), Color(),"font" , "Jamie", Color(1.0f,0.0f,0.0f,0.0f), true, ZENI_LEFT, 5)
  {

		//textures
		Sprite GroundSprite;
		Sprite TargetSprite;
		Sprite CrosshairSprite;
		Sprite Crosshair2Sprite;
		Sprite SkySprite;
		Sprite WallSprite;
		Sprite RealCrosshairsSprite;


		GroundSprite.append_frame("Ground");
		TargetSprite.append_frame("Target");
		CrosshairSprite.append_frame("Crosshairs");
		Crosshair2Sprite.append_frame("Crosshairs2");
		SkySprite.append_frame("Sky");
		WallSprite.append_frame("Wall");
		RealCrosshairsSprite.append_frame("RealCrosshairs");

		Textures::get_reference().set_texture("GroundSprite", new Sprite(GroundSprite));
		Textures::get_reference().set_texture("TargetSprite", new Sprite(TargetSprite));
		Textures::get_reference().set_texture("CrosshairSprite", new Sprite(CrosshairSprite));
		Textures::get_reference().set_texture("SkySprite", new Sprite(SkySprite));
		Textures::get_reference().set_texture("WallSprite", new Sprite(WallSprite));
		Textures::get_reference().set_texture("Crosshair2Sprite", new Sprite(Crosshair2Sprite));
		Textures::get_reference().set_texture("RealCrosshairsSprite", new Sprite(RealCrosshairsSprite));

		//music handling
		Sound &sr = Sound::get_reference();
		sr.set_BGM("music/music.ogg");
		sr.play_BGM();
		sr.set_BGM_looping(false);

		//misc
		ZSpeed = 0.0f;
		bInvert = true;
		m_shooting = false;
		m_pull = false;
		iPlayerScore = 0;
		iPlace= 0;

		//sets sound sources
		ArrowShotSound = new Sound_Source(Sounds::get_reference().get_sound("Shoot"));
		ArrowHitSound = new Sound_Source(Sounds::get_reference().get_sound("Hit"));
		ArrowPullSound = new Sound_Source(Sounds::get_reference().get_sound("Pull"));
		
		for (int CurrentTarget = 0; CurrentTarget < TARGETS_IN_USE; ++CurrentTarget)
		{
			SetTargets(CurrentTarget);
		}

		iTime = 60;
		iPlace = 0;
		bInvert = false;
		PlayerName = "";
		TBox.seek(0);
  }

  void Crate_State::on_key(const SDL_KeyboardEvent &event) {
	  // WASD for moving, SPACE for jumping

	  //handles editing the text for the text box
	  if (iPlace > 0)
	  {
		string sOld = TBox.get_text();
		TBox.on_key(event.keysym, (event.type == SDL_KEYDOWN));
		if (TBox.get_num_lines() > 1) {
			  TBox.set_text(sOld);
			  if(TBox.get_edit_pos() > 0)
				  TBox.seek(TBox.get_edit_pos() - 1);
		}

		if (event.keysym.sym == SDLK_RETURN)
		{
			PlayerName = TBox.get_text();
		}
	  }


		switch(event.keysym.sym) 
		{
			case SDLK_ESCAPE:
				if (event.type == SDL_KEYDOWN)
				{
					Game::get_reference().pop_state();
					Game::get_reference().push_state(new Title::Title_State);
				}
				break;
			case SDLK_w:
				m_controls.forward = (event.type == SDL_KEYDOWN);
				break;
			case SDLK_s:
				m_controls.back = (event.type == SDL_KEYDOWN);
				break;
			case SDLK_a:
				m_controls.left = (event.type == SDL_KEYDOWN);
				break;
			case SDLK_d:
				m_controls.right = (event.type == SDL_KEYDOWN);
				break;
			case SDLK_SPACE:
				if((m_player.is_on_ground()) && (event.type == SDL_KEYDOWN))
				{
					m_player.set_on_ground(false);
					m_player.set_velocity(Vector3f(m_player.get_velocity().i,m_player.get_velocity().j,PLAYER_SPEED));
				}
				break;
			case SDLK_i:
					if(event.type == SDL_KEYDOWN)
					{

						if (bInvert)
							bInvert = false;
						else
							bInvert = true;
					}
				break;
			default:
				Gamestate_Base::on_key(event);
				break;
		}
  }

  void Crate_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
	 //allows inverted or normal camera (FPS STYLE)
	if (bInvert)
	{
		m_player.adjust_pitch(-event.yrel * 0.005f );
		if(m_player.get_camera().get_up().k < 0)
			m_player.adjust_pitch(event.yrel * 0.005f);
	}
	else
	{
		m_player.adjust_pitch(event.yrel * 0.005f );
		if(m_player.get_camera().get_up().k < 0)
			m_player.adjust_pitch(-event.yrel * 0.005f);
	}

	  m_player.turn_left_xy(-event.xrel * 0.005f);

  }

  void Crate_State::on_mouse_button(const SDL_MouseButtonEvent &event) {

	if((event.button == 1) && (!m_shooting))
	{
		ArrowPullSound->play();
		m_pull = true;
		if (event.type == SDL_MOUSEBUTTONUP)
		{
		m_shooting = true;
		m_pull = false;
		}
	}
  }

  void Crate_State::perform_logic() {
		//get a good time step value -- time since last perfom_logic() call
		const Time_HQ current_time = Timer_HQ::get_reference().get_time();
		float time_step = float(current_time.get_seconds_since(time_passed));

		time_passed = current_time;

		if (time_step > 0.1f)
			time_step = 0.1f;

		for(float substep = 0.01f; time_step > 0.0f; time_step -= substep)
		{
			if (substep > time_step)
				substep = time_step;


			if (iTime > 0)
			{
			float VectorZ = m_player.get_velocity().k;

			m_player.set_velocity(Vector3f(0.0f,0.0f,0.0f));

			float Pspeed = 0.0f;

			//sets the player velocity
			if (m_controls.forward)
					Pspeed = PLAYER_SPEED;

			if(m_controls.back)
					Pspeed = -PLAYER_SPEED;

			partial_step(substep, Vector3f(Pspeed, 0.0f, 0.0f));

			Pspeed = 0.0f;

			if(m_controls.left)
					Pspeed = PLAYER_SPEED;

			if(m_controls.right)
					Pspeed = -PLAYER_SPEED;

			partial_step(substep, Vector3f(0.0f, Pspeed, 0.0f));

			Pspeed = 0.0f;

			if((!m_player.is_on_ground()) || (m_player.get_camera().get_position().z > GROUND))
			{
				ZSpeed = VectorZ - PLAYER_SPEED*GRAVITY*time_step;

				if (ZSpeed < -MAX_FALL_SPEED)
				{
					ZSpeed = -MAX_FALL_SPEED;
				}

				Pspeed = ZSpeed;
				
			}

			if(m_player.get_camera().get_position().z < GROUND)
			{
				m_player.set_velocity(Vector3f(0.0f,0.0f,0.0f));
				m_player.set_position(Point3f(m_player.get_camera().get_position().x,m_player.get_camera().get_position().y,GROUND));
				m_player.set_on_ground(true);
				VectorZ = 0.0f; 
				Pspeed = 0.0f;
			}

			partial_step(substep, Vector3f(0.0f, 0.0f, Pspeed));

			//sound data stuff
			Sound &sr = Sound::get_reference();

			if (sr.get_BGM_time() > 83.25f)
			{
				sr.set_BGM_time(17.0f);
			}

			//handles arrow
			if(m_shooting)
				ShootArrow();

			int iTimeBefore = int(iTime);
			iTime -= substep;

			//controls targets
			ControlTargets(substep);

			if (int(iTime) < iTimeBefore)
				--iPlayerScore;

			}


			if (PlayerName != "")
				AddScores();

			if (iTime <= 0)
			{
				if (iPlace == 0)
					iPlace = CheckScores();
			}
		}
  }

  void Crate_State::render() {
	  Video &vr = Video::get_reference();

	  vr.set_3d(m_player.get_camera());

	  m_crate.render();

	  m_fog.set_density(0.001);
	  m_fog.set_type(FOG_EXP2);

	  vr.set_fog(&m_fog);

		DrawSkyBox();																																																																																																																																																																													
		vr.set_3d(m_player.get_camera());

	  //renders the floor
		{
			Vertex3f_Texture ul(Point3f(0.0f,0.0f, 0.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,1000.0f, 0.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,8.0f));
			Vertex3f_Texture lr(Point3f(1000.0f,1000.0f, 0.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(8.0f,8.0f));
			Vertex3f_Texture ur(Point3f(1000.0f,0.0f, 0.0f), Vector3f(8.0f,0.0f, 0.0f),			Point2f(8.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Floor(ul, ll, lr, ur, new Material_Render_Wrapper(Material("GroundSprite")));

			Video::get_reference().render(Floor);

		}

		RenderTargets();
		DrawWalls();


		//draws the crosshairs
		{
			vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(SCREEN_WIDTH, SCREEN_HEIGHT)));

			if (m_pull)
			{
			Vertex2f_Texture ul(Point2f(0.0f, SCREEN_HEIGHT / 2 - 256 + 64),	Point2f(0.0f,0.0f));
			Vertex2f_Texture ll(Point2f(0.0f,SCREEN_HEIGHT / 2 + 256 + 64),				Point2f(0.0f,1.0f));
			Vertex2f_Texture lr(Point2f(1024.0f, SCREEN_HEIGHT / 2 + 256 + 64),	Point2f(1.0f,1.0f));
			Vertex2f_Texture ur(Point2f(1024.0f, SCREEN_HEIGHT / 2 - 256 + 64),				Point2f(1.0f,0.0f));

			Quadrilateral<Vertex2f_Texture> quad(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Crosshair2Sprite")));

			Video::get_reference().render(quad);

			}
			else
			{
			Vertex2f_Texture ul(Point2f(0.0f, SCREEN_HEIGHT / 2 - 256 + 64),	Point2f(0.0f,0.0f));
			Vertex2f_Texture ll(Point2f(0.0f,SCREEN_HEIGHT / 2 + 256 + 64),				Point2f(0.0f,1.0f));
			Vertex2f_Texture lr(Point2f(1024.0f, SCREEN_HEIGHT / 2 + 256 + 64),	Point2f(1.0f,1.0f));
			Vertex2f_Texture ur(Point2f(1024.0f, SCREEN_HEIGHT / 2 - 256 + 64),				Point2f(1.0f,0.0f));

			Quadrilateral<Vertex2f_Texture> quad(ul, ll, lr, ur, new Material_Render_Wrapper(Material("CrosshairSprite")));

			Video::get_reference().render(quad);
			}

			Vertex2f_Texture ul(Point2f(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 16),	Point2f(0.0f,0.0f));
			Vertex2f_Texture ll(Point2f(SCREEN_WIDTH / 2 - 16,SCREEN_HEIGHT / 2 + 16),				Point2f(0.0f,1.0f));
			Vertex2f_Texture lr(Point2f(SCREEN_WIDTH / 2 + 16, SCREEN_HEIGHT / 2 + 16),	Point2f(1.0f,1.0f));
			Vertex2f_Texture ur(Point2f(SCREEN_WIDTH / 2 + 16, SCREEN_HEIGHT / 2 - 16),				Point2f(1.0f,0.0f));

			Quadrilateral<Vertex2f_Texture> quad(ul, ll, lr, ur, new Material_Render_Wrapper(Material("RealCrosshairsSprite")));

			Video::get_reference().render(quad);

		}

		Zeni::Color m_color = Color(1.0f,1.0f,0.0f,0.0f);

		//text stuff
		vr.set_2d();

	  	Fonts &fr = Fonts::get_reference(); // get access to Font database
		Font &font = fr.get_font("font");

	  	{
			ostringstream oss; // can be treated like cout
			oss << "Directions:\n";

			if (m_controls.forward)
				oss << "Forward: true.\n";
			else
				oss << "Forward: false.\n";

			if (m_controls.back)
				oss << "Back: true.\n";
			else
				oss << "Back: false.\n";

			if (m_controls.left)
				oss << "Left: true.\n";
			else
				oss << "Left: false.\n";

			if (m_controls.right)
				oss << "Right: true.\n\n";
			else
				oss << "Right: false.\n\n";

			if(m_player.is_on_ground())
				oss << "On ground: true.\n\n";
			else
				oss << "On ground: false.\n\n";

			if(m_shooting)
				oss << "Shooting: true.\n\n";
			else
				oss << "Shooting: false.\n\n";

			if (ArrowHit)
				oss << "\nArrow Hit.";
			else
				oss << "\nArrow Not Hit.";

			font.render_text(oss.str(), 10, 1, m_color, ZENI_LEFT);
		}	

		//renders the fps
		{
			ostringstream oss; // can be treated like cout
			if (bInvert)
				oss << "Camera: Inverted.\n";
			else
				oss << "Camera: Normal.\n";

			oss << "FPS: " << Game::get_reference().get_fps() << "\n\n Co-ordinates:\n" 
				<< "X: " << m_player.get_camera().get_position().x
				<< "\nY: " << m_player.get_camera().get_position().y
				<< "\nZ: " << m_player.get_camera().get_position().z;

			oss << "\n\nZSpeed: " << ZSpeed;

			oss << "\n\nX Forward: " << m_player.get_camera().get_forward().i;
			oss << "\nY Forward: " << m_player.get_camera().get_forward().j;
			oss << "\nZ Forward: " << m_player.get_camera().get_forward().k;

			font.render_text(oss.str(), vr.get_screen_width() - 10, 1, m_color, ZENI_RIGHT);
		}

		//renders the score
		{
			ostringstream oss; // can be treated like cout
			oss << "Score: " << iPlayerScore;
			oss << "\nTime: " << int(iTime);
			oss << "\nPlace: " << iPlace;

			font.render_text(oss.str(), vr.get_screen_width()/2, 1, m_color, ZENI_CENTER);
		}

		if (iPlace > 0)
			TBox.render();
  }

  void Crate_State::DrawWalls()
  {

		//sets the left wall
	  {
			Vertex3f_Texture ul(Point3f(0.0f,1000.0f, 200.0f),Vector3f(8.0f,.0f, 0.0f),				Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,1000.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),				Point2f(0.0f,1.0f));
			Vertex3f_Texture lr(Point3f(1000.0f,1000.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,1.0f));
			Vertex3f_Texture ur(Point3f(1000.0f,1000.0f, 200.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Wall")));

			Video::get_reference().render(Sky);
	  }

		//sets the right wall
	  {
			Vertex3f_Texture ul(Point3f(1000.0f,0.0f, 200.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(1000.0f,0.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,1.0f));
			Vertex3f_Texture lr(Point3f(0.0f,0.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),				Point2f(2.0f,1.0f));
			Vertex3f_Texture ur(Point3f(0.0f,0.0f, 200.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Wall")));

			Video::get_reference().render(Sky);
	  }

	  //sets the front wall
	  {
			Vertex3f_Texture ul(Point3f(1000.0f,1000.0f, 200.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(1000.0f,1000.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,1.0f));
			Vertex3f_Texture lr(Point3f(1000.0f,0.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),				Point2f(2.0f,1.0f));
			Vertex3f_Texture ur(Point3f(1000.0f,0.0f, 200.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Wall")));

			Video::get_reference().render(Sky);
	  }


	  //sets the back wall
	  {
			Vertex3f_Texture ul(Point3f(0.0f,1000.0f, 200.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,1000.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,1.0f));
			Vertex3f_Texture lr(Point3f(0.0f,0.0f, 0.0f),Vector3f(8.0f,.0f, 0.0f),				Point2f(2.0f,1.0f));
			Vertex3f_Texture ur(Point3f(0.0f,0.0f, 200.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Wall")));

			Video::get_reference().render(Sky);
	  }

  }

  void Crate_State::DrawSkyBox()
  {
	  Video &vr = Video::get_reference();
		Camera skycam = m_player.get_camera();
		skycam.set_position(Point3f(1000.0f, 1000.0f,50.0f));

		vr.set_3d(skycam);
		vr.set_zwrite(false);

	  //sets the top part
	  {
			Vertex3f_Texture ul(Point3f(0.0f,0.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,2000.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(2000.0f,2000.0f, 650.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(2000.0f,0.0f, 650.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Sky);
	  }

	  	//draws bottom part
		{
			Vertex3f_Texture ul(Point3f(0.0f,0.0f, -350.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,2000.0f, -350.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(2000.0f,2000.0f, -350.0f),Vector3f(8.0f,0.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(2000.0f,0.0f, -350.0f), Vector3f(8.0f,0.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Floor(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Floor);

		}

		//sets the left wall
	  {
			Vertex3f_Texture ul(Point3f(0.0f,2000.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,2000.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(2000.0f,2000.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(2000.0f,2000.0f, 750.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Sky);
	  }

		//sets the right wall
	  {
			Vertex3f_Texture ul(Point3f(2000.0f,0.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(2000.0f,0.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(0.0f,0.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(0.0f,0.0f, 750.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Sky);
	  }

	  //sets the front wall
	  {
			Vertex3f_Texture ul(Point3f(2000.0f,2000.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(2000.0f,2000.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(2000.0f,0.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(2000.0f,0.0f, 750.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Sky);
	  }


	  //sets the back wall
	  {
			Vertex3f_Texture ul(Point3f(0.0f,2000.0f, 750.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,0.0f));
			Vertex3f_Texture ll(Point3f(0.0f,2000.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(0.0f,2.0f));
			Vertex3f_Texture lr(Point3f(0.0f,0.0f, -350.0f),Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,2.0f));
			Vertex3f_Texture ur(Point3f(0.0f,0.0f, 750.0f), Vector3f(8.0f,.0f, 0.0f),			Point2f(2.0f,0.0f));

			Quadrilateral<Vertex3f_Texture> Sky(ul, ll, lr, ur, new Material_Render_Wrapper(Material("Sky")));

			Video::get_reference().render(Sky);
	  }

		vr.set_zwrite(true);
  }

  void Crate_State::partial_step(const float &time_step, const Vector3f &velocity) {
	  // for later
		Point3f LastPoint = m_player.get_camera().get_position();

		m_player.set_velocity(velocity);

		bool intersect = m_player.get_body().intersects(m_crate.get_body());

		m_player.step(time_step);

		//plays intersect sound
		if (intersect)
			m_crate.collide();

		if((m_player.get_camera().get_position().x > 990.0f) || (m_player.get_camera().get_position().x < 10.0f) || 
			(m_player.get_camera().get_position().y > 990.0f) || (m_player.get_camera().get_position().y < 10.0f))
		{
			m_player.set_position(LastPoint);
		}

		if (m_player.get_body().intersects(m_crate.get_body()))
		{
			m_player.set_position(LastPoint);

			if (velocity.k != 0.0f)
			{
				m_player.set_on_ground(true);
			}
		}
  }

  void Crate_State::ShootArrow()
  {
	//makes the shoot arrow sound
	ArrowShotSound->play();

	Point3f ArrowPos = m_player.get_camera().get_position();
	Vector3f ArrowVel = m_player.get_camera().get_forward();

	Zeni_Collision::Ray m_arrow = Ray(ArrowPos, ArrowVel);

	for (int CurrentTarget = 0; CurrentTarget < TARGETS_IN_USE; ++CurrentTarget)
	{
		Zeni_Collision::Sphere m_body;

		//creates the sphere based on the angle of the target
		if((m_targets[CurrentTarget].Aim == Target::DIAGONAL) || (m_targets[CurrentTarget].Aim == Target::DIAGONAL2))
			m_body = Sphere(Point3f(m_targets[CurrentTarget].x + 25,m_targets[CurrentTarget].y + 25,m_targets[CurrentTarget].z - 25), 25.0f);
		else if (m_targets[CurrentTarget].Aim == Target::VERTICAL)
			m_body = Sphere(Point3f(m_targets[CurrentTarget].x + 25,m_targets[CurrentTarget].y + 50,m_targets[CurrentTarget].z - 25), 25.0f);
		else if (m_targets[CurrentTarget].Aim == Target::HORIZONTAL)
			m_body = Sphere(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 25,m_targets[CurrentTarget].z - 25), 25.0f);

		if(m_arrow.intersects(m_body))
		{
			//plays the sound of hitting, set hit as true, and exit
			ArrowHitSound->play();
			ArrowHit = true;
			iPlayerScore += int((m_body.shortest_distance(m_player.get_body()) / 25) * ((abs(m_targets[CurrentTarget].TSpeed.i) / 25) + (abs(m_targets[CurrentTarget].TSpeed.j) / 25) + (abs(m_targets[CurrentTarget].TSpeed.k) / 25)) / 3);
			SetTargets(CurrentTarget);
			iTime++;
			break;
		}
		else
		{
			//else no hit
			ArrowHit = false;
		}
	}

	if (ArrowHit == false)
	{
		iPlayerScore -= 10;
		--iTime;
	}

	m_shooting = false;
  }


  /**************************************************************************************************************************************************************/

  void Crate_State::SetTargets(int CurrentTarget)
  {	
	  	//sets stupid targets
			int iSet = rand() % 6 + 1; //used for position
			int iMove = rand() % 3 + 1; //used for movement
			if ((iSet == 1) || (iSet == 5))
			{
				int iTest = rand() % 2 + 1; 

				if (iTest == 1)
					m_targets[CurrentTarget].x = 1000 + (rand() % 300 + 1.0f);
				else
					m_targets[CurrentTarget].x = -(rand() % 300 + 100.0f);
				m_targets[CurrentTarget].y = rand() % 500 + 300.0f;
				m_targets[CurrentTarget].z = rand() % 600 + 500.0f;
				m_targets[CurrentTarget].Aim = Target::HORIZONTAL;
				
				if (iMove == 3)
				{
					m_targets[CurrentTarget].TSpeed = Vector3f(0.0f, rand() % 100 + 50.0f, 0.0f);
					m_targets[CurrentTarget].EndPoint = Point3f(m_targets[CurrentTarget].x, 1750, m_targets[CurrentTarget].z);
					m_targets[CurrentTarget].Respawn = Point3f(m_targets[CurrentTarget].x, -750, m_targets[CurrentTarget].z);	
				}
				else if (iMove == 4)
				{
					m_targets[CurrentTarget].TSpeed = Vector3f(0.0f, -(rand() % 100 + 50.0f), 0.0f);
					m_targets[CurrentTarget].EndPoint = Point3f(m_targets[CurrentTarget].x, -750, m_targets[CurrentTarget].z);
					m_targets[CurrentTarget].Respawn = Point3f(m_targets[CurrentTarget].x, 1750, m_targets[CurrentTarget].z);	
				}

			}
			else if ((iSet == 2) || (iSet == 6))
			{
				int iTest = rand() % 2 + 1;

				m_targets[CurrentTarget].x = rand() % 500 + 300.0f;
				if (iTest == 1)
					m_targets[CurrentTarget].y = -(rand() % 300 + 100.0f);
				else
					m_targets[CurrentTarget].y = 1000 + (rand() % 300 + 1.0f);
				m_targets[CurrentTarget].z = rand() % 600 + 500.0f;
				m_targets[CurrentTarget].Aim = Target::VERTICAL;

				if (iMove == 3)
				{
					m_targets[CurrentTarget].TSpeed = Vector3f(rand() % 100 + 50.0f, 0.0f, 0.0f);
					m_targets[CurrentTarget].EndPoint = Point3f(1750, m_targets[CurrentTarget].y, m_targets[CurrentTarget].z);
					m_targets[CurrentTarget].Respawn = Point3f(-750, m_targets[CurrentTarget].y, m_targets[CurrentTarget].z);	
				}
				else if (iMove == 4)
				{
					m_targets[CurrentTarget].TSpeed = Vector3f(-(rand() % 100 + 50.0f), 0.0f, 0.0f);
					m_targets[CurrentTarget].EndPoint = Point3f(-750, m_targets[CurrentTarget].y, m_targets[CurrentTarget].z);
					m_targets[CurrentTarget].Respawn = Point3f(1750, m_targets[CurrentTarget].y, m_targets[CurrentTarget].z);	
				}

			}
			else if (iSet == 3)
			{
				int iTest = rand() % 2 + 1;

				if (iTest == 1)
				{
					m_targets[CurrentTarget].x = -100 -(rand() % 500);
					m_targets[CurrentTarget].y = -100 -(rand() % 500);
				}
				else
				{
					m_targets[CurrentTarget].x = 1000 + rand() % 500 + 100.0f;
					m_targets[CurrentTarget].y = 1000 + rand() % 500 + 100.0f;
				}

				m_targets[CurrentTarget].z = rand() % 600 + 500.0f;
				m_targets[CurrentTarget].Aim = Target::DIAGONAL;

				iMove = rand() % 2 + 1;
			}
			else if (iSet == 4)
			{
				int iTest = rand() % 2 + 1;

				if (iTest == 1)
				{
					m_targets[CurrentTarget].x = 1500 -(rand() % 500 + 100.0f);
					m_targets[CurrentTarget].y = -(rand() % 500 + 100.0f);
				}
				else
				{
					m_targets[CurrentTarget].x = rand() % 500 + 100.0f;
					m_targets[CurrentTarget].y = 1000 + rand() % 500 + 100.0f;
				}

				m_targets[CurrentTarget].z = rand() % 600 + 500.0f;
				m_targets[CurrentTarget].Aim = Target::DIAGONAL2;

				iMove = rand() % 2 + 1;
			}

		if (iMove == 1)
		{
			m_targets[CurrentTarget].TSpeed = Vector3f(0.0f, 0.0f, rand() % 100 + 50.0f);
			m_targets[CurrentTarget].EndPoint = Point3f(m_targets[CurrentTarget].x, m_targets[CurrentTarget].y, 1300.0f);
			m_targets[CurrentTarget].Respawn = Point3f(m_targets[CurrentTarget].x, m_targets[CurrentTarget].y, 150.0f);	
		}
		else if (iMove == 2)
		{
			m_targets[CurrentTarget].TSpeed = Vector3f(0.0f, 0.0f, -(rand() % 100 + 50.0f));
			m_targets[CurrentTarget].EndPoint = Point3f(m_targets[CurrentTarget].x, m_targets[CurrentTarget].y, 150.0f);
			m_targets[CurrentTarget].Respawn = Point3f(m_targets[CurrentTarget].x, m_targets[CurrentTarget].y, 1300.0f);
		}
  }
  void Crate_State::RenderTargets()
  {
	  Video &vr = Video::get_reference();
	  vr.set_3d(m_player.get_camera());

		//loads the very lame targets
	  for (int CurrentTarget = 0; CurrentTarget < TARGETS_IN_USE; ++CurrentTarget)
		{

			Vertex3f_Texture ul;
			Vertex3f_Texture ur;
			Vertex3f_Texture ll;
			Vertex3f_Texture lr;

			//gets the angle of the target
			
			if (m_targets[CurrentTarget].Aim == Target::DIAGONAL)
			{
				ul = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
				ll = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,1.0f));
				lr = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,1.0f));
				ur = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,0.0f));
			}
			else if (m_targets[CurrentTarget].Aim == Target::DIAGONAL2)
			{
				ul = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
				ll = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,1.0f));
				lr = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,1.0f));
				ur = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,0.0f));
			}
			else if (m_targets[CurrentTarget].Aim == Target::VERTICAL)
			{
				ul = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
				ll = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,1.0f));
				lr = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,1.0f));
				ur = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,0.0f));
			}
			else if (m_targets[CurrentTarget].Aim == Target::HORIZONTAL)
			{
				ul = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,0.0f));
				ll = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y + 50, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(0.0f,1.0f));
				lr = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z),		Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,1.0f));
				ur = Vertex3f_Texture(Point3f(m_targets[CurrentTarget].x + 50,m_targets[CurrentTarget].y, m_targets[CurrentTarget].z - 50),	Vector3f(8.0f,0.0f, 0.0f),			Point2f(1.0f,0.0f));
			}

			Quadrilateral<Vertex3f_Texture> Target(ul, ll, lr, ur, new Material_Render_Wrapper(Material("TargetSprite")));

			Video::get_reference().render(Target);

		}
  }

  void Crate_State::ControlTargets(const float &time_step)
  {
		//handles various target info, such as moving
		for (int CurrentTarget = 0; CurrentTarget < TARGETS_IN_USE; ++CurrentTarget)
		{
			m_targets[CurrentTarget].x += m_targets[CurrentTarget].TSpeed.i * time_step;
			m_targets[CurrentTarget].y += m_targets[CurrentTarget].TSpeed.j * time_step;
			m_targets[CurrentTarget].z += m_targets[CurrentTarget].TSpeed.k * time_step;

			//checks if the target is farther than the end point, if so, reset
			if((m_targets[CurrentTarget].TSpeed.k > 0.0f) && (m_targets[CurrentTarget].z > m_targets[CurrentTarget].EndPoint.z) || (m_targets[CurrentTarget].TSpeed.k < 0.0f) && (m_targets[CurrentTarget].z < m_targets[CurrentTarget].EndPoint.z))
				m_targets[CurrentTarget].z = m_targets[CurrentTarget].Respawn.z;

			if((m_targets[CurrentTarget].TSpeed.j > 0.0f) && (m_targets[CurrentTarget].y > m_targets[CurrentTarget].EndPoint.y) || (m_targets[CurrentTarget].TSpeed.j < 0.0f) && (m_targets[CurrentTarget].y < m_targets[CurrentTarget].EndPoint.y))
				m_targets[CurrentTarget].y = m_targets[CurrentTarget].Respawn.y;

			if((m_targets[CurrentTarget].TSpeed.i > 0.0f) && (m_targets[CurrentTarget].x > m_targets[CurrentTarget].EndPoint.x) || (m_targets[CurrentTarget].TSpeed.i < 0.0f) && (m_targets[CurrentTarget].x < m_targets[CurrentTarget].EndPoint.x))
				m_targets[CurrentTarget].x = m_targets[CurrentTarget].Respawn.x;
		}
  }

  /**************************************************************************************************************************************************************/

  void Crate_State::AddScores()
  {
	  
	  	int NumLines = 0;
		ifstream fin("highscores.txt");
		string player[11];
		int score[11];
		while (fin.good())
		{
			ostringstream oss;
			string line;
			getline(fin, line);

			if(line.empty())
				continue;

			int split = int(line.find_first_of("||"));

			player[NumLines] = line.substr(0, split);
			score[NumLines] = atoi(line.substr(split+2).c_str());
			++NumLines;
		}
		fin.close();


		for (int i = 11 - 2; i >= (iPlace-1); i--)
		{
			player[i+1] = player[i];
			score[i+1] = score[i];
		}

		player[iPlace - 1] = PlayerName;
		score[iPlace - 1] = iPlayerScore;

		ofstream fout("highscores.txt");

		for (int iTemp = 0; iTemp < 10; ++iTemp)
		{
			string strput;
			if (player[iTemp] != "")
			{
				strput.append(player[iTemp]); 
				strput.append("||"); 
				stringstream out;
				out << score[iTemp];
				strput.append(out.str());
				strput.append("\n");
			}
			fout.write(strput.c_str(), strput.length());
		}

		fin.close();
		iPlace = 0;
		Game::get_reference().pop_state();
		Game::get_reference().push_state(new Title::Title_State);
  }

  int Crate_State::CheckScores()
  {
	  	int NumLines = 0;
		ifstream fin("highscores.txt");
		while (fin.good())
		{
			++NumLines;
			ostringstream oss;
			string line;
			getline(fin, line);

			if(line.empty())
				continue;

			int split = line.find_first_of("||");

			string player = line.substr(0, split);
			int score = atoi(line.substr(split+2).c_str());

			if (iPlayerScore > score)
			{
				fin.close();
				return NumLines;
			}
		}
		fin.close();
		return 0;
  }
}
