  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include <boost/noncopyable.hpp>

#include <SDL.h>
#include <string>
#include <vector>
#include <cassert>
#include <memory>
#include <iostream> // NOLINT for error reporting when messagebox has failed.

#include "euphoria/game.h"
#include "euphoria/exception.h"
#include "euphoria/settings.h"

void Error(const std::string& title, const std::string& text) {
  const int result = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                     title.c_str(), text.c_str(), NULL);
  if (result < 0) {
    std::cerr << "ERROR: " << title << "\n" << text << "\n";
  }
}

void ReportFail() {
  const std::string error = SDL_GetError();
  throw error;
}

void HandleStatus(int code) {
  if (code < 0) {
    ReportFail();
  }
}

class Sdl : boost::noncopyable {
  public:
    Sdl() {
      assert(this);
      HandleStatus(SDL_Init(SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK
                            | SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    }

    ~Sdl() {
      assert(this);
      SDL_Quit();
    }
};

class VideoMode {
  public:
    explicit VideoMode(const SDL_DisplayMode& mode)
      : width_(mode.w)
      , height_(mode.h)
      , refreshRate_(mode.refresh_rate)
      , pixel_format_(SDL_GetPixelFormatName(mode.format)) {
      assert(this);
    }

    int width() const {
      assert(this);
      return width_;
    }

    int height() const {
      assert(this);
      return height_;
    }

    int refreshRate() const {
      assert(this);
      return refreshRate_;
    }

    const std::string& pixel_format() const {
      assert(this);
      return pixel_format_;
    }

  private:
    int width_;
    int height_;
    int refreshRate_;
    std::string pixel_format_;
};

class DisplayInfo {
  public:
    explicit DisplayInfo(int id) {
      assert(this);
      SDL_Rect bounds;
      HandleStatus(SDL_GetDisplayBounds(id, &bounds));
      width_ = bounds.w;
      height_ = bounds.h;
      x_ = bounds.x;
      y_ = bounds.y;

      const char* name = SDL_GetDisplayName(id);
      if (name != 0) {
        name_ = name;
      }

      const int nummodes = SDL_GetNumDisplayModes(id);
      if (nummodes <= 0) {
        ReportFail();
        return;
      }
      for (int modeid = 0; modeid < nummodes; ++modeid) {
        SDL_DisplayMode mode;
        HandleStatus(SDL_GetDisplayMode(id, modeid, &mode));
        modes_.push_back(VideoMode(mode));
      }
    }

    int width() const {
      assert(this);
      return width_;
    }

    int height() const {
      assert(this);
      return height_;
    }

    int x() const {
      assert(this);
      return x_;
    }

    int y() const {
      assert(this);
      return y_;
    }

    const std::string& name() {
      assert(this);
      return name_;
    }

    const std::vector<VideoMode>& modes() const {
      assert(this);
      return modes_;
    }

  private:
    int width_;
    int height_;
    int x_;
    int y_;
    std::string name_;
    std::vector<VideoMode> modes_;
};

class VideoDisplays {
  public:
    VideoDisplays() {
      assert(this);
      const int numberofdisplays = SDL_GetNumVideoDisplays();
      if (numberofdisplays <= 0) {
        ReportFail();
        return;
      }
      for (int displayid = 0; displayid < numberofdisplays; ++displayid) {
        displays_.push_back(DisplayInfo(displayid));
      }
    }

    const std::vector<DisplayInfo>& displays() const {
      assert(this);
      return displays_;
    }

  private:
    std::vector<DisplayInfo> displays_;
};

class Window : boost::noncopyable {
  public:
    Window(const std::string title, int x, int y, int width, int height,
           bool main)
      : window_(SDL_CreateWindow(title.c_str(), x, y, width, height,
                                 main ? (SDL_WINDOW_OPENGL
                                         | SDL_WINDOW_INPUT_FOCUS
                                         | SDL_WINDOW_MOUSE_FOCUS)
                                 : SDL_WINDOW_BORDERLESS))
      , width_(width), height_(height) {
      assert(this);
      if (window_ == NULL) {
        ReportFail();
      }
    }

    ~Window() {
      assert(this);
      if (window_) {
        SDL_DestroyWindow(window_);
        window_ = 0;
      }
    }

    int width() const {
      return width_;
    }

    int height() const {
      return height_;
    }

    SDL_Window* window() {
      assert(this);
      return window_;
    }

    void SetupDisplayModeToDesktop() {
      assert(this);
      assert(window_);
      int result = SDL_SetWindowDisplayMode(window_, NULL);
      if (result < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to set display mode: %d",
                     SDL_GetWindowID(window_));
      }
    }

    bool SetFullscreen(bool truefullscreen) {
      if (truefullscreen) {
        const int fullscreen = SDL_SetWindowFullscreen(window_,
                               SDL_WINDOW_FULLSCREEN);
        if (fullscreen < 0) {
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                      "Failed to set true full screen: %d - %s",
                      SDL_GetWindowID(window_), SDL_GetError());
          const int fullscreendesktop = SDL_SetWindowFullscreen(window_,
                                        SDL_WINDOW_FULLSCREEN_DESKTOP);
          if (fullscreendesktop < 0) {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to set fake full screen: %d - %s",
                        SDL_GetWindowID(window_), SDL_GetError());
          }
          return fullscreendesktop >= 0;
        } else {
          return true;
        }
      } else {
        const int fullscreendesktop = SDL_SetWindowFullscreen(window_,
                                      SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (fullscreendesktop < 0) {
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                      "Setting window fake fullscreen: %d - %s",
                      SDL_GetWindowID(window_), SDL_GetError());
        }
        return fullscreendesktop >= 0;
      }
    }

  private:
    SDL_Window* window_;
    int width_;
    int height_;
};

class BlackRenderer : boost::noncopyable {
  public:
    explicit BlackRenderer(Window* window)
      : renderer_(SDL_CreateRenderer(window->window(), -1,
                                     SDL_RENDERER_ACCELERATED
                                     | SDL_RENDERER_PRESENTVSYNC)) {
      assert(this);
      if (renderer_ == NULL) {
        ReportFail();
      }
      SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    }

    void Render() {
      assert(this);
      SDL_RenderClear(renderer_);
      SDL_RenderPresent(renderer_);
    }

    ~BlackRenderer() {
      assert(this);
      SDL_DestroyRenderer(renderer_);
    }

  private:
    SDL_Renderer* renderer_;
};

class Timer {
  public:
    Timer() : start_(0) {
      Reset();
    }

    void Reset() {
      start_ = SDL_GetPerformanceCounter();
    }

    template<typename F>
    F GetElapsedSeconds() const {
      const Uint64 now = SDL_GetPerformanceCounter();
      return static_cast<F>(now - start_) / SDL_GetPerformanceFrequency();
    }

  private:
    Uint64 start_;
};

typedef std::pair<int, int> Posi;
Posi GetHatValues(Uint8 hat) {
  switch (hat) {
    case SDL_HAT_LEFTUP:
      return Posi(-1, 1);
    case SDL_HAT_UP:
      return Posi(0, 1);
    case SDL_HAT_RIGHTUP  :
      return Posi(1, 1);
    case SDL_HAT_LEFT:
      return Posi(-1, 0);
    case SDL_HAT_CENTERED:
      return Posi(0, 0);
    case SDL_HAT_RIGHT:
      return Posi(1, 0);
    case SDL_HAT_LEFTDOWN:
      return Posi(-1, -1);
    case SDL_HAT_DOWN:
      return Posi(0, -1);
    case SDL_HAT_RIGHTDOWN:
      return Posi(1, -1);
    default:
      assert(0 && "Invalid hat value");
      return Posi(0, 0);
  }
}

class Joystick {
  public:
    explicit Joystick(int id) : id_(id), joystick_(SDL_JoystickOpen(id))
      , numHats_(0) {
      if (joystick_ == NULL) {
        throw "Failed to open joystick";
      }

      numHats_ = SDL_JoystickNumHats(joystick_);

      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                  "Opened joystick %s on %d: #axes: %d /"
                  " #buttons: %d / #balls: %d / #hats: %d",
                  SDL_JoystickNameForIndex(id_),
                  id_,
                  SDL_JoystickNumAxes(joystick_),
                  SDL_JoystickNumButtons(joystick_),
                  SDL_JoystickNumBalls(joystick_),
                  numHats_);
    }

    ~Joystick() {
      if (IsAttached()) {
        SDL_JoystickClose(joystick_);
        joystick_ = NULL;
      }
    }

    bool IsAttached() const {
      return SDL_JoystickGetAttached(joystick_) == SDL_TRUE;
    }

    void SendPov(Game* game) {
      if (IsAttached()) {
        for (int i = 0; i < numHats_; ++i) {
          const Uint8 hat = SDL_JoystickGetHat(joystick_, i);
          const Posi values = GetHatValues(hat);
          game->OnAxis(Axis::JoystickPovX, id_,
                       static_cast<float>(values.first));
          game->OnAxis(Axis::JoystickPovY, id_,
                       static_cast<float>(values.second));
        }
      }
    }

  private:
    int id_;
    int numHats_;
    SDL_Joystick* joystick_;
};

class Context : boost::noncopyable {
  public:
    explicit Context(Window* window)
      : window_(window->window()), context_(SDL_GL_CreateContext(
                                              window->window())) {
      assert(this);
      assert(window);
      assert(window_);
      if (context_ == NULL) {
        ReportFail();
      }
    }

    ~Context() {
      assert(this);
      SDL_GL_DeleteContext(context_);
    }

    void Swap() {
      SDL_GL_SwapWindow(window_);
    }

    void MakeCurrent() {
      const int ret = SDL_GL_MakeCurrent(window_, context_);
      if (ret < 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed changing the context: %d - %s",
                    SDL_GetWindowID(window_), SDL_GetError());
      }
    }

  private:
    SDL_Window* window_;
    SDL_GLContext context_;
};

Key::Type ToKey(SDL_JoyButtonEvent button) {
  switch (button.button) {
    case 0:
      return Key::JoystickButton1;
    case 1:
      return Key::JoystickButton2;
    case 2:
      return Key::JoystickButton3;
    case 3:
      return Key::JoystickButton4;
    case 4:
      return Key::JoystickButton5;
    case 5:
      return Key::JoystickButton6;
    case 6:
      return Key::JoystickButton7;
    case 7:
      return Key::JoystickButton8;
    case 8:
      return Key::JoystickButton9;
    case 9:
      return Key::JoystickButton10;
    case 10:
      return Key::JoystickButton11;
    case 11:
      return Key::JoystickButton12;
    case 12:
      return Key::JoystickButton13;
    case 13:
      return Key::JoystickButton14;
    case 14:
      return Key::JoystickButton15;
    case 15:
      return Key::JoystickButton16;
    case 16:
      return Key::JoystickButton17;
    case 17:
      return Key::JoystickButton18;
    case 18:
      return Key::JoystickButton19;
    case 19:
      return Key::JoystickButton20;
    case 20:
      return Key::JoystickButton21;
    case 21:
      return Key::JoystickButton22;
    case 22:
      return Key::JoystickButton23;
    case 23:
      return Key::JoystickButton24;
    case 24:
      return Key::JoystickButton25;
    case 25:
      return Key::JoystickButton26;
    case 26:
      return Key::JoystickButton27;
    case 27:
      return Key::JoystickButton28;
    case 28:
      return Key::JoystickButton29;
    case 29:
      return Key::JoystickButton30;
    case 30:
      return Key::JoystickButton31;
    case 31:
      return Key::JoystickButton32;
    default:
      assert(0 && "Invalid joystick button");
      return Key::Invalid;
  }
}

Key::Type ToKey(SDL_Keysym key) {
  switch (key.sym) {
    case SDLK_a:
      return Key::A;
    case SDLK_b:
      return Key::B;
    case SDLK_c:
      return Key::C;
    case SDLK_d:
      return Key::D;
    case SDLK_e:
      return Key::E;
    case SDLK_f:
      return Key::F;
    case SDLK_g:
      return Key::G;
    case SDLK_h:
      return Key::H;
    case SDLK_i:
      return Key::I;
    case SDLK_j:
      return Key::J;
    case SDLK_k:
      return Key::K;
    case SDLK_l:
      return Key::L;
    case SDLK_m:
      return Key::M;
    case SDLK_n:
      return Key::N;
    case SDLK_o:
      return Key::O;
    case SDLK_p:
      return Key::P;
    case SDLK_q:
      return Key::Q;
    case SDLK_r:
      return Key::R;
    case SDLK_s:
      return Key::S;
    case SDLK_t:
      return Key::T;
    case SDLK_u:
      return Key::U;
    case SDLK_v:
      return Key::V;
    case SDLK_w:
      return Key::W;
    case SDLK_x:
      return Key::X;
    case SDLK_y:
      return Key::Y;
    case SDLK_z:
      return Key::Z;
    case SDLK_0:
      return Key::Num0;
    case SDLK_1:
      return Key::Num1;
    case SDLK_2:
      return Key::Num2;
    case SDLK_3:
      return Key::Num3;
    case SDLK_4:
      return Key::Num4;
    case SDLK_5:
      return Key::Num5;
    case SDLK_6:
      return Key::Num6;
    case SDLK_7:
      return Key::Num7;
    case SDLK_8:
      return Key::Num8;
    case SDLK_9:
      return Key::Num9;
    case SDLK_ESCAPE:
      return Key::Escape;
    case SDLK_LCTRL:
      return Key::LControl;
    case SDLK_LSHIFT:
      return Key::LShift;
    case SDLK_LALT:
      return Key::LAlt;
    case SDLK_LGUI:
      return Key::LSystem;
    case SDLK_RCTRL:
      return Key::RControl;
    case SDLK_RSHIFT:
      return Key::RShift;
    case SDLK_RALT:
      return Key::RAlt;
    case SDLK_RGUI:
      return Key::RSystem;
    case SDLK_MENU:
      return Key::Menu;
    case SDLK_LEFTBRACKET:
      return Key::LBracket;
    case SDLK_RIGHTBRACKET:
      return Key::RBracket;
    case SDLK_SEMICOLON:
      return Key::SemiColon;
    case SDLK_COMMA:
      return Key::Comma;
    case SDLK_PERIOD:
      return Key::Period;
    case SDLK_QUOTE:
      return Key::Quote;
    case SDLK_SLASH:
      return Key::Slash;
    case SDLK_BACKSLASH:
      return Key::BackSlash;
    case SDLK_BACKQUOTE:
      return Key::Tilde;
    case SDLK_EQUALS:
      return Key::Equal;
    case SDLK_MINUS:
      return Key::Dash;
    case SDLK_SPACE:
      return Key::Space;
    case SDLK_RETURN:
      return Key::Return;
    case SDLK_BACKSPACE:
      return Key::BackSpace;
    case SDLK_TAB:
      return Key::Tab;
    case SDLK_PAGEUP:
      return Key::PageUp;
    case SDLK_PAGEDOWN:
      return Key::PageDown;
    case SDLK_END:
      return Key::End;
    case SDLK_HOME:
      return Key::Home;
    case SDLK_INSERT:
      return Key::Insert;
    case SDLK_DELETE:
      return Key::Delete;
    case SDLK_KP_PLUS:
      return Key::Add;
    case SDLK_KP_MINUS:
      return Key::Subtract;
    case SDLK_KP_MULTIPLY:
      return Key::Multiply;
    case SDLK_KP_DIVIDE:
      return Key::Divide;
    case SDLK_LEFT:
      return Key::Left;
    case SDLK_RIGHT:
      return Key::Right;
    case SDLK_UP:
      return Key::Up;
    case SDLK_DOWN:
      return Key::Down;
    case SDLK_KP_0:
      return Key::Numpad0;
    case SDLK_KP_1:
      return Key::Numpad1;
    case SDLK_KP_2:
      return Key::Numpad2;
    case SDLK_KP_3:
      return Key::Numpad3;
    case SDLK_KP_4:
      return Key::Numpad4;
    case SDLK_KP_5:
      return Key::Numpad5;
    case SDLK_KP_6:
      return Key::Numpad6;
    case SDLK_KP_7:
      return Key::Numpad7;
    case SDLK_KP_8:
      return Key::Numpad8;
    case SDLK_KP_9:
      return Key::Numpad9;
    case SDLK_F1 :
      return Key::F1;
    case SDLK_F2 :
      return Key::F2;
    case SDLK_F3 :
      return Key::F3;
    case SDLK_F4 :
      return Key::F4;
    case SDLK_F5 :
      return Key::F5;
    case SDLK_F6 :
      return Key::F6;
    case SDLK_F7 :
      return Key::F7;
    case SDLK_F8 :
      return Key::F8;
    case SDLK_F9 :
      return Key::F9;
    case SDLK_F10:
      return Key::F10;
    case SDLK_F11:
      return Key::F11;
    case SDLK_F12:
      return Key::F12;
    case SDLK_F13:
      return Key::F13;
    case SDLK_F14:
      return Key::F14;
    case SDLK_F15:
      return Key::F15;
    case SDLK_PAUSE:
      return Key::Pause;
    default:
      assert(0 && key.sym && "Invalid keyboard button");
      return Key::Invalid;
  }
}

Axis::Type ToAxis(SDL_JoyAxisEvent joy) {
  switch (joy.axis) {
    case 0:
      return Axis::JoystickX;
    case 1:
      return Axis::JoystickY;
    case 2:
      return Axis::JoystickZ;
    case 3:
      return Axis::JoystickR;
    case 4:
      return Axis::JoystickU;
    case 5:
      return Axis::JoystickV;
    default:
      assert(0 && "Invalid joystick axis");
      return Axis::MouseY;
  }
}

int FindOculusDisplay(const VideoDisplays& displays) {
  for (size_t i = 0; i < displays.displays().size(); ++i) {
    DisplayInfo di = displays.displays()[i];
    if (di.name() == "Rift DK1") {
      return i;
    }
  }
  return -1;
}

int DeterminePrimaryDisplayId(const Settings& settings,
                              const VideoDisplays& displays
                              , bool* foundoculus) {
  assert(foundoculus);
  if (settings.oculus_vr_detection() == OculusVrDetection::Auto) {
    const int vrid = FindOculusDisplay(displays);
    if (vrid != -1) {
      *foundoculus = true;
      return vrid;
    } else {
      *foundoculus = false;
      return settings.primary_display_id();
    }
  } else {
    *foundoculus = settings.oculus_vr_detection()
                   == OculusVrDetection::Oculusvr;
    return settings.primary_display_id();
  }
}

void logic() {
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
  Settings settings;
  settings.Load();

  Sdl sdl;
  SDL_DisableScreenSaver();
  VideoDisplays displays;

  std::shared_ptr<Window> primaryscreen;
  std::vector<std::shared_ptr<Window> > windows;
  std::vector<std::shared_ptr<BlackRenderer> > blacks;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  // The core profile causes http://www.opengl.org/wiki/GLAPI/glGenVertexArrays
  // to crash, weird...

  bool renderoculus = false;

  int displayid = DeterminePrimaryDisplayId(settings, displays, &renderoculus);

  for (size_t i = 0; i < displays.displays().size(); ++i) {
    const bool isprimaryscrreen = i == displayid;
    const bool createscreen = isprimaryscrreen || settings.blackout();

    if (createscreen) {
      DisplayInfo di = displays.displays()[i];

      const int width = isprimaryscrreen == false || settings.width() == 0 ?
                        di.width() : settings.width();
      const int height = isprimaryscrreen == false || settings.height() == 0 ?
                         di.height() : settings.height();
      if (width > di.width()) {
        throw "Target width is too large";
      }
      if (height > di.height()) {
        throw "height is too large";
      }

      const int x = di.x() + (di.width() - width) / 2;
      const int y = di.y() + (di.height() - height) / 2;
      std::shared_ptr<Window> screen(new Window(isprimaryscrreen ?
                                     "Game" : "Black", x, y, width, height,
                                     isprimaryscrreen));
      windows.push_back(screen);

      screen->SetupDisplayModeToDesktop();

      if (isprimaryscrreen) {
        primaryscreen = screen;
      } else {
        std::shared_ptr<BlackRenderer> br(new BlackRenderer(screen.get()));
        blacks.push_back(br);
      }
    }
  }

  if (renderoculus) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Renders with the oculus vr");
  } else {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Renders normally without vr support");
  }

  if (primaryscreen.get() == NULL) {
    throw "Unable to find primary screen";
  }

  Context context(primaryscreen.get());
  if (settings.fullscreen()) {
    /* Setting the black windows fullscreen messes with the primary fullscreen
    for(auto screen:windows) {
        if( screen != primaryscreen ){
            screen->SetFullscreen(false);
        }
    }*/
    primaryscreen->SetFullscreen(true);
  }

  std::vector<std::shared_ptr<Joystick>> joysticks;

  const int numberofjoysticks = SDL_NumJoysticks();
  for (int i = 0; i < numberofjoysticks; ++i) {
    const char* const name = SDL_JoystickNameForIndex(i);
    if (name == NULL) {
      ReportFail();
    }
    std::shared_ptr<Joystick> js(new Joystick(i));
    joysticks.push_back(js);
  }

  Timer timer;

  context.MakeCurrent();
  Game game(settings, renderoculus);

  while (game.keep_running()) {
    const float delta = timer.GetElapsedSeconds<float>();
    timer.Reset();
    game.Update(delta);

    for (auto black : blacks) {
      black->Render();
    }

    context.MakeCurrent();
    game.Render();
    context.Swap();

    SDL_Event event;

    int xrel = 0;
    int yrel = 0;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        game.Quit();
      } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        const bool down = event.type == SDL_KEYDOWN;
        game.OnKey(ToKey(event.key.keysym), 0, down);
      } else if (event.type == SDL_MOUSEMOTION) {
        xrel += event.motion.xrel;
        yrel += event.motion.yrel;
      } else if (event.type == SDL_JOYBUTTONDOWN
                 || event.type == SDL_JOYBUTTONUP) {
        const bool down = event.type == SDL_JOYBUTTONDOWN;
        game.OnKey(ToKey(event.jbutton), event.jbutton.which, down);
      } else if (event.type == SDL_JOYAXISMOTION) {
        game.OnAxis(ToAxis(event.jaxis),
                    event.jaxis.which,
                    event.jaxis.value / 32768.0f);
      } else if (event.type == SDL_WINDOWEVENT) {
        const auto mouseEvent = event.window.event;
        if (mouseEvent == SDL_WINDOWEVENT_ENTER
            || mouseEvent == SDL_WINDOWEVENT_LEAVE) {
          const bool lock = mouseEvent == SDL_WINDOWEVENT_ENTER;
          SDL_SetRelativeMouseMode(lock ? SDL_TRUE : SDL_FALSE);
        }
      }
    }
    for (auto js : joysticks) {
      js->SendPov(&game);
    }

    const float size = static_cast<float>(std::max(primaryscreen->height(),
                                          primaryscreen->width()));
    float dx = xrel / size;
    float dy = yrel / size;
    const float sensitivity = 10.0f;
    game.OnAxis(Axis::MouseX, 0, dx * sensitivity);
    game.OnAxis(Axis::MouseY, 0, dy * sensitivity);
  }
}


int main(int argc, char* argv[]) {
  try {
    logic();
    return 0;
  } catch (...) {
    const std::string message = GrabExceptionInformation();
    Error("Error!", message);
    return 1;
  }
}
