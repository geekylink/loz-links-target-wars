#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "Crate.h"
#include "Player.h"
#include <Zeni/Fog.h>
#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>
#include <Zeni/Widget.h>

namespace Title {

  class Title_State : public Zeni::Gamestate_Base {
    
  public:
    Title_State();

	void render();

	void on_key(const SDL_KeyboardEvent &event);
    
  };

}

#endif
