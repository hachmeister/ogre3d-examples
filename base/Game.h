#pragma once

#include <OgreSceneManager.h>
#include <SDL2/SDL_events.h>

extern "C" struct SDL_Window;

namespace Ogre {
class Root;
class RenderWindow;
class Camera;
}  // namespace Ogre

namespace MyGUI {
class Gui;
class Ogre2Platform;
}  // namespace MyGUI

class Game {
public:
  Game();

  virtual ~Game();

  void run();

protected:
  void processInput();

  virtual void handleEvent(SDL_Event& event) = 0;

  virtual void update(float time) = 0;

  Ogre::SceneManager& sceneManager() const;

  MyGUI::Gui& gui() const;

  void shutdown();

private:
  void resizeWindow(unsigned int width, unsigned int height);

private:
  bool shutdown_;

  Ogre::Root* root_;
  Ogre::RenderWindow* renderWindow_;
  Ogre::SceneManager* sceneManager_;
  Ogre::Camera* camera_;

  SDL_Window* window_;

  MyGUI::Gui* gui_;
  MyGUI::Ogre2Platform* platform_;
};
