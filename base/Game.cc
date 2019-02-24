#include "Game.h"

#include <MyGUI.h>
#include "mygui/MyGUI_Ogre2Platform.h"

#include <Compositor/OgreCompositorManager2.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>
#include <Hlms/Unlit/OgreHlmsUnlit.h>
#include <Ogre.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

Game::Game()
    : shutdown_(false),
      root_(nullptr),
      renderWindow_(nullptr),
      sceneManager_(nullptr),
      camera_(nullptr),
      window_(nullptr),
      gui_(nullptr),
      platform_(nullptr) {
  // init Ogre
  root_ = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");

  if (!root_->restoreConfig()) {
    if (!root_->showConfigDialog()) {
      throw std::runtime_error("Could not restore config/show config dialog!");
    }
  }

  root_->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
  root_->initialise(false);

  Ogre::ConfigOptionMap& opts = root_->getRenderSystem()->getConfigOptions();

  bool fullscreen = Ogre::StringConverter::parseBool(opts["Full Screen"].currentValue);
  int width = 1280;
  int height = 720;

  Ogre::ConfigOptionMap::iterator opt = opts.find("Video Mode");
  if (opt != opts.end()) {
    const Ogre::String::size_type start = opt->second.currentValue.find_first_of("012356789");
    Ogre::String::size_type widthEnd = opt->second.currentValue.find(' ', start);
    Ogre::String::size_type heightEnd = opt->second.currentValue.find(' ', widthEnd + 3);
    width = Ogre::StringConverter::parseInt(opt->second.currentValue.substr(0, widthEnd));
    height =
        Ogre::StringConverter::parseInt(opt->second.currentValue.substr(widthEnd + 3, heightEnd));
  }

  Ogre::String windowTitle = "Test";
  window_ = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height,
      SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);

  if (SDL_GetWindowWMInfo(window_, &wmInfo) == SDL_FALSE) {
    throw std::runtime_error("Couldn't get WM Info! (SDL2)");
  }

  Ogre::String winHandle = Ogre::StringConverter::toString((uintptr_t)wmInfo.info.x11.window);

  Ogre::NameValuePairList params;
  params.insert(std::make_pair("parentWindowHandle", winHandle));
  params.insert(std::make_pair("title", windowTitle));
  params.insert(std::make_pair("gamma", "true"));
  params.insert(std::make_pair("FSAA", opts["FSAA"].currentValue));
  params.insert(std::make_pair("vsync", opts["VSync"].currentValue));

  renderWindow_ = root_->createRenderWindow(windowTitle, width, height, fullscreen, &params);

  sceneManager_ =
      root_->createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD);

  // add resources
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
      "assets/models", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/MyGUI_Media", "FileSystem",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

  // setup hlms
  Ogre::HlmsUnlit* hlmsUnlit = 0;
  Ogre::HlmsPbs* hlmsPbs = 0;

  Ogre::String rootPath = "assets/";
  Ogre::String mainPath;
  Ogre::StringVector libraryPaths;

  Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

  {
    Ogre::HlmsUnlit::getDefaultPaths(mainPath, libraryPaths);

    Ogre::Archive* archiveUnlit = archiveManager.load(rootPath + mainPath, "FileSystem", true);
    Ogre::ArchiveVec archiveUnlitPaths;

    for (Ogre::String path : libraryPaths) {
      Ogre::Archive* archiveLibrary = archiveManager.load(rootPath + path, "FileSystem", true);
      archiveUnlitPaths.push_back(archiveLibrary);
    }

    hlmsUnlit = new Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitPaths);
    Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);
  }

  {
    Ogre::HlmsPbs::getDefaultPaths(mainPath, libraryPaths);

    Ogre::Archive* archivePbs = archiveManager.load(rootPath + mainPath, "FileSystem", true);
    Ogre::ArchiveVec archivePbsPaths;

    for (Ogre::String path : libraryPaths) {
      Ogre::Archive* archiveLibrary = archiveManager.load(rootPath + path, "FileSystem", true);
      archivePbsPaths.push_back(archiveLibrary);
    }

    hlmsPbs = new Ogre::HlmsPbs(archivePbs, &archivePbsPaths);
    Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);
  }

  // init resources
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);

  // create camera
  camera_ = sceneManager_->createCamera("Camera");
  camera_->setPosition(Ogre::Vector3(0, 0, 4));
  camera_->lookAt(Ogre::Vector3(0, 0, 0));
  camera_->setNearClipDistance(0.2f);
  camera_->setFarClipDistance(1000.0f);
  camera_->setAutoAspectRatio(true);

  // setup MyGUI
  platform_ = new MyGUI::Ogre2Platform();
  platform_->initialise(renderWindow_, sceneManager_);

  gui_ = new MyGUI::Gui();
  gui_->initialise();

  MyGUI::PointerManager::getInstance().setVisible(false);

  // setup workspace
  Ogre::CompositorManager2* compositorManager = root_->getCompositorManager2();
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
      "assets/compositors", "FileSystem", "Compositors");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Compositors", true);
  compositorManager->addWorkspace(sceneManager_, renderWindow_, camera_, "SceneWorkspace", true);
}

Game::~Game() {
  if (gui_) {
    gui_->shutdown();
    delete gui_;
  }

  if (platform_) {
    platform_->shutdown();
    delete platform_;
  }
}

void Game::run() {
  Ogre::Timer timer;
  unsigned long startTime = timer.getMicroseconds();
  double timeSinceLast = 1.0 / 60.0;

  while (!shutdown_) {
    processInput();
    update(timeSinceLast);

    Ogre::WindowEventUtilities::messagePump();

    if (renderWindow_->isVisible()) {
      shutdown_ |= !root_->renderOneFrame();
    }

    unsigned long endTime = timer.getMicroseconds();
    timeSinceLast = (endTime - startTime) / 1000000.0;
    startTime = endTime;
  }
}

void Game::processInput() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_MOUSEMOTION:
      MyGUI::InputManager::getInstance().injectMouseMove(event.motion.x, event.motion.y, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      MyGUI::InputManager::getInstance().injectMousePress(
          event.button.x, event.button.y, MyGUI::MouseButton::Enum(0));
      break;
    case SDL_MOUSEBUTTONUP:
      MyGUI::InputManager::getInstance().injectMouseRelease(
          event.button.x, event.button.y, MyGUI::MouseButton::Enum(0));
      break;
    case SDL_KEYDOWN:
      MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(event.key.keysym.sym));
      break;
    case SDL_KEYUP:
      MyGUI::InputManager::getInstance().injectKeyRelease(
          MyGUI::KeyCode::Enum(event.key.keysym.sym));
      break;
    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        resizeWindow(event.window.data1, event.window.data2);
        break;
      }
      break;
    }

    handleEvent(event);
  }
}

Ogre::SceneManager& Game::sceneManager() const {
  return *sceneManager_;
}

MyGUI::Gui& Game::gui() const {
  return *gui_;
}

void Game::shutdown() {
  shutdown_ = true;
}

void Game::resizeWindow(unsigned int width, unsigned int height) {
  renderWindow_->resize(width, height);
  camera_->setAspectRatio((float)width / (float)height);
}
