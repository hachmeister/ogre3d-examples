#pragma once

#include "Game.h"

namespace MyGUI {
class Widget;
}

class Gui : public Game {
public:
  Gui();

  ~Gui();

  void onButton(MyGUI::Widget* widget);

protected:
  void handleEvent(SDL_Event& event);

  void update(float time);
};
