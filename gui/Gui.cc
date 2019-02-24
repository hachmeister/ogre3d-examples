#include "Gui.h"

#include <MyGUI.h>

Gui::Gui() {
  // add gui elements
  MyGUI::ButtonPtr button =
      gui().createWidget<MyGUI::Button>("Button", 0, 0, 256, 32, MyGUI::Align::Default, "Main");
  button->setCaption("exit");
  button->setTextColour(MyGUI::Colour(1, 1, 1));
  button->eventMouseButtonClick += MyGUI::newDelegate(this, &Gui::onButton);
}

Gui::~Gui() {}

void Gui::onButton(MyGUI::Widget* widget) {
  shutdown();
}

void Gui::handleEvent(SDL_Event& event) {
  switch (event.type) {
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      shutdown();
      break;
    }
    break;
  case SDL_QUIT:
    shutdown();
    break;
  }
}

void Gui::update(float time) {}
