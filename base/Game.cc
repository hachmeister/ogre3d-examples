#include "Game.h"

#include <Compositor/OgreCompositorManager2.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>
#include <Ogre.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

Game::Game()
    : shutdown_(false),
      root_(nullptr),
      renderWindow_(nullptr),
      sceneManager_(nullptr),
      camera_(nullptr),
      window_(nullptr) {
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

  // setup hlms
  Ogre::HlmsPbs* hlmsPbs = 0;

  Ogre::String rootPath = "assets/";

  Ogre::String mainPath;
  Ogre::StringVector libraryPaths;

  Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

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

  // setup workspace
  Ogre::CompositorManager2* compositorManager = root_->getCompositorManager2();
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
      "assets/compositors", "FileSystem", "Compositors");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Compositors", true);
  compositorManager->addWorkspace(sceneManager_, renderWindow_, camera_, "SceneWorkspace", true);
}

Game::~Game() {}

void Game::run() {
  Ogre::Timer timer;
  unsigned long startTime = timer.getMicroseconds();
  double timeSinceLast = 1.0 / 60.0;

  while (!shutdown_) {
    processInput();
    update(timeSinceLast);

    Ogre::WindowEventUtilities::messagePump();

    if (renderWindow_->isVisible()) {
      // shutdown_ |= root_->renderOneFrame();
      root_->renderOneFrame();
    }

    unsigned long endTime = timer.getMicroseconds();
    timeSinceLast = (endTime - startTime) / 1000000.0;
    startTime = endTime;
  }
}

void Game::processInput() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    handleEvent(e);
  }
}

Ogre::SceneManager& Game::sceneManager() const {
  return *sceneManager_;
}

void Game::shutdown() {
  shutdown_ = true;
}
