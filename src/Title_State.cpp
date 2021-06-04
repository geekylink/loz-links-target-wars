#include "Title_State.h"
#include "Crate_State.h"

#include <Zeni/Camera.hxx>
#include <Zeni/Collision.hxx>
#include <Zeni/Timer.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Sounds.h>
#include <Zeni/Widget.hxx>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Zeni;
using namespace Zeni_Collision;

#define SCREEN_HEIGHT 600.0f
#define SCREEN_WIDTH 800.0f

namespace Title {

	static bool bLoad;
	static short int iSelect;

  Title_State::Title_State()
  {
		//plays the title screen song
		Sound &sr = Sound::get_reference();
		sr.set_BGM("music/title.ogg");
		sr.play_BGM();
		sr.set_BGM_looping(false);

		Sprite TitleScreen;
		TitleScreen.append_frame("Title");
		Textures::get_reference().set_texture("TitleScreen", new Sprite(TitleScreen));

		bLoad = false;
		iSelect = 0;
  }

  void Title_State::render()
  {
		Video &vr = Video::get_reference();
		vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(SCREEN_WIDTH, SCREEN_HEIGHT)));

		Vertex2f_Texture ul(Point2f(-16.0f, 0.0f),							Point2f(0.0f,0.0f));
		Vertex2f_Texture ll(Point2f(-16.0f,SCREEN_HEIGHT),					Point2f(0.0f,1.0f));
		Vertex2f_Texture lr(Point2f(SCREEN_WIDTH + 128,SCREEN_HEIGHT ),		Point2f(1.0f,1.0f));
		Vertex2f_Texture ur(Point2f(SCREEN_WIDTH + 128,0.0f ),				Point2f(1.0f,0.0f));

		Quadrilateral<Vertex2f_Texture> quad(ul, ll, lr, ur, new Material_Render_Wrapper(Material("TitleScreen")));
		vr.render(quad);

		vr.set_2d();

		Fonts &fr = Fonts::get_reference(); // get access to Font database
		Font &font = fr.get_font("font2");

		Zeni::Color m_color = Color(1.0f, 0.0f, 1.0f, 0.0f);
		//renders text
		if ((bLoad) && (iSelect == 0))
		{
			ostringstream oss; // can be treated like cout
			oss << "Loading...";

			font.render_text(oss.str(), vr.get_screen_height() / 2,  vr.get_screen_width() / 2, Color(1.0,0.0f,1.0f,0.0f), ZENI_CENTER);
			Game::get_reference().pop_state();
			Game::get_reference().push_state(new Crate::Crate_State);
		}
		else if ((bLoad) && (iSelect == 1))
		{
			int NumLines = 0;
				ifstream fin("highscores.txt");

									vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(SCREEN_WIDTH, SCREEN_HEIGHT)));

					Vertex2f_Color ul(Point2f(vr.get_screen_width() * 0.55, vr.get_screen_height() * 0.25),			Color(0.5f,0.0f,0.0f,0.0f));
					Vertex2f_Color ll(Point2f(vr.get_screen_width() * 0.55,vr.get_screen_height()),					Color(0.5f,0.0f,0.0f,0.0f));
					Vertex2f_Color lr(Point2f(vr.get_screen_width(),vr.get_screen_height()),						Color(0.5f,0.0f,0.0f,0.0f));
					Vertex2f_Color ur(Point2f(vr.get_screen_width(),vr.get_screen_height() * 0.25),					Color(0.5f,0.0f,0.0f,0.0f));

					Quadrilateral<Vertex2f_Color> quad(ul, ll, lr, ur);
					vr.render(quad);

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

					for(int iTemp = NumLines;iTemp > 1; --iTemp)
					{
						oss << "\n";
					}
					oss << NumLines << ". " << player << ": " << score;

					vr.set_2d();
					font.render_text(oss.str(), vr.get_screen_width() * 0.75,  vr.get_screen_height() * 0.25, Color(1.0,0.0f,1.0f,0.0f), ZENI_CENTER);
				}
			fin.close();
			//font.render_text(oss.str(), vr.get_screen_height() / 2,  vr.get_screen_width() / 2, Color(), ZENI_CENTER);
		}
		else if ((bLoad) && (iSelect == 2))
		{
			ostringstream oss; // can be treated like cout
			oss << "I LOL @ YOU!";

			font.render_text(oss.str(), vr.get_screen_height() / 2,  vr.get_screen_width() / 2, Color(), ZENI_CENTER);
		}
		else if ((bLoad) && (iSelect == 3))
		{
			throw Quit_Event();
		}
		else
		{
			{
				ostringstream oss; // can be treated like cout
				if (iSelect == 0)
					oss << "*** Start ***";
				else
					oss << "Start";
				font.render_text(oss.str(), int(vr.get_screen_width() * 0.2f),  int(vr.get_screen_height() * 0.85f), m_color, ZENI_CENTER);
			}
			{
				ostringstream oss; // can be treated like cout
				if (iSelect == 1)
					oss << "*** Highscores ***";
				else
					oss << "Highscores";
				font.render_text(oss.str(), int(vr.get_screen_width() * 0.5f),  int(vr.get_screen_height() * 0.85f), m_color, ZENI_CENTER);
			}
			{
				ostringstream oss; // can be treated like cout
				if (iSelect == 2)
					oss << "\n*** Options ***";
				else
					oss << "\nOptions";
				font.render_text(oss.str(), int(vr.get_screen_width() * 0.2f),  int(vr.get_screen_height() * 0.85f), m_color, ZENI_CENTER);
			}
			{
				ostringstream oss; // can be treated like cout
				if (iSelect == 3)
					oss << "\n*** Exit ***";
				else
					oss << "\nExit";
				font.render_text(oss.str(), int(vr.get_screen_width() * 0.5f),  int(vr.get_screen_height() * 0.85f), m_color, ZENI_CENTER);
			}
			bLoad = false;
		}
  }

  void Title_State::on_key(const SDL_KeyboardEvent &event)
  {
		if ((event.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYDOWN))
			throw Quit_Event();

	  //rest of key events
		if ((event.keysym.sym == SDLK_RETURN) && (event.type == SDL_KEYDOWN))
		{
			bLoad = true;
		}
		else if ((event.keysym.sym == SDLK_UP) && (event.type == SDL_KEYDOWN))
		{
			if (iSelect > 1)
				iSelect -= 2;
		}
		else if ((event.keysym.sym == SDLK_DOWN) && (event.type == SDL_KEYDOWN))
		{
			if (iSelect <= 1)
				iSelect += 2;
		}
		else if ((event.keysym.sym == SDLK_RIGHT) && (event.type == SDL_KEYDOWN))
		{
			if ((iSelect == 0) || (iSelect == 2))
				++iSelect;
		}
		else if ((event.keysym.sym == SDLK_LEFT) && (event.type == SDL_KEYDOWN))
		{
			if ((iSelect == 1) || (iSelect == 3))
				--iSelect;
		}
		else
			Gamestate_Base::on_key(event);
  }

}
