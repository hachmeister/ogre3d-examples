#include "Cube.h"

#include <OgreLight.h>
#include <OgreMeshManager2.h>

Cube::Cube() {
  // add light
  Ogre::Light* light = sceneManager().createLight();
  light->setPowerScale(Ogre::Math::PI);
  light->setType(Ogre::Light::LT_DIRECTIONAL);

  Ogre::SceneNode* lightNode = sceneManager().getRootSceneNode()->createChildSceneNode();
  lightNode->attachObject(light);
  lightNode->setPosition(0, 10, 0);
  lightNode->setDirection({0, -1, -1});

  // add cube
  Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load(
      "Cube.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  Ogre::Item* item = sceneManager().createItem(mesh);

  cubeNode_ = sceneManager().getRootSceneNode()->createChildSceneNode();
  cubeNode_->attachObject((Ogre::MovableObject*)item);
}

Cube::~Cube() {}

void Cube::handleEvent(SDL_Event& event) {
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

void Cube::update(float time) {
  cubeNode_->rotate({0, -1, 0}, Ogre::Radian{0.5f * time});
}
