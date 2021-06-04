#ifndef CRATE_STATE_H
#define CRATE_STATE_H

#include "Crate.h"
#include "Player.h"
#include <Zeni/Fog.h>
#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>
#include <Zeni/Widget.h>

#define TARGETS_IN_USE 99

#define SCREEN_HEIGHT 600.0f
#define SCREEN_WIDTH 800.0f

namespace Crate {

  class Crate_State : public Zeni::Gamestate_Base {
    struct Controls {
      Controls() : forward(false), left(false), back(false), right(false) {}

      bool forward;
      bool left;
      bool back;
      bool right;
    } m_controls;

  public:
    Crate_State();

    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);
	void on_mouse_button(const SDL_MouseButtonEvent &event);

    void perform_logic();

	void DrawSkyBox();
	void DrawWalls();

    void render();
	void RenderTargets(); // renders the targets

	void ShootArrow(); // Shoots an arrow. Returns 0 for wall, 1 for target, and 2 for off the map

	void SetTargets(int); //Picks the spots for all the targets

	void ControlTargets(const float &);

	void AddScores();
	int CheckScores();

  private:
    void partial_step(const float &time_step, const Zeni::Vector3f &velocity);

    Zeni::Time_HQ time_passed;

    Crate m_crate;
    Player m_player;

	//fog
	Zeni::Fog m_fog;

    bool m_moved;

	//mine
	bool bInvert;
	bool m_shooting;
	bool m_pull;
	int iPlayerScore;
	int iPlace;
	float ZSpeed;

	float iTime;

	Zeni::Text_Box TBox;
  };


  //target structure

    
  typedef struct  {
		enum AIM_TYPE {DIAGONAL = 0, DIAGONAL2 = 1, VERTICAL = 2, HORIZONTAL = 3};

		// control the position of the target
		float x;
		float y;
		float z;

		AIM_TYPE Aim; // controls angle of target

		Zeni::Vector3f TSpeed;
		Zeni::Point3f Respawn;
		Zeni::Point3f EndPoint;
    } Target;

}

#endif
