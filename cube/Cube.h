#pragma once

#include "Game.h"

class Cube : public Game {
public:
  Cube();

  ~Cube();

protected:
  void handleEvent(SDL_Event& event);

  void update(float time);

private:
  Ogre::SceneNode* cubeNode_;
};
