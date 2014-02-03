// Euphoria - Copyright (c) Gustav

#include <boost/noncopyable.hpp>
#include <SDL.h>
#include <AntTweakBar.h>

#include <string>
#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>
#include <iostream>  // NOLINT for error reporting when messagebox has failed.

#include "euphoria/game.h"
#include "euphoria/exception.h"
#include "euphoria/settings.h"

#ifdef WIN32
// unresolved external symbol _GetFileVersionInfoA@16 referenced in function
// _IME_GetId
#pragma comment(lib, "Version.lib")
// unresolved external symbol _ImmGetIMEFileNameA@12 referenced in function
// _IME_GetId
#pragma comment(lib, "Imm32.lib")
#endif

void Error(const std::string& title, const std::string& text) {
  const int result = SDL_ShowSimpleMessageBox(
      SDL_MESSAGEBOX_ERROR, title.c_str(), text.c_str(), NULL);
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
  explicit Sdl(const Settings& settings) {
    assert(this);
    const Uint32 flags = settings.support_joystick()
                             ? SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK |
                                   SDL_INIT_VIDEO | SDL_INIT_EVENTS
                             : SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    HandleStatus(SDL_Init(flags));
  }

  ~Sdl() {
    assert(this);
    SDL_Quit();
  }
};

class VideoMode {
 public:
  explicit VideoMode(const SDL_DisplayMode& mode)
      : width_(mode.w),
        height_(mode.h),
        refreshRate_(mode.refresh_rate),
        pixel_format_(SDL_GetPixelFormatName(mode.format)) {
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
  Window(const std::string& title, int x, int y, int width, int height,
         bool main)
      : window_(SDL_CreateWindow(
            title.c_str(), x, y, width, height,
            main ? (SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS |
                    SDL_WINDOW_MOUSE_FOCUS)
                 : SDL_WINDOW_BORDERLESS)),
        width_(width),
        height_(height) {
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

  int width() const { return width_; }

  int height() const { return height_; }

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
                   "Failed to set display mode: %d", SDL_GetWindowID(window_));
    }
  }

  bool SetFullscreen(bool truefullscreen) {
    if (truefullscreen) {
      const int fullscreen =
          SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
      if (fullscreen < 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to set true full screen: %d - %s",
                    SDL_GetWindowID(window_), SDL_GetError());
        const int fullscreendesktop =
            SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
      const int fullscreendesktop =
          SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
      : renderer_(SDL_CreateRenderer(
            window->window(), -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
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
  Timer() : start_(0) { Reset(); }

  void Reset() { start_ = SDL_GetPerformanceCounter(); }

  template <typename F>
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
    case SDL_HAT_RIGHTUP:
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
  explicit Joystick(int id)
      : id_(id), joystick_(SDL_JoystickOpen(id)), numHats_(0) {
    if (joystick_ == NULL) {
      throw "Failed to open joystick";
    }

    numHats_ = SDL_JoystickNumHats(joystick_);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Opened joystick %s on %d: #axes: %d /"
                " #buttons: %d / #balls: %d / #hats: %d",
                SDL_JoystickNameForIndex(id_), id_,
                SDL_JoystickNumAxes(joystick_),
                SDL_JoystickNumButtons(joystick_),
                SDL_JoystickNumBalls(joystick_), numHats_);
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
        game->inputsystem().OnJoystickPov(Axis::X, id_,
                                          static_cast<float>(values.first));
        game->inputsystem().OnJoystickPov(Axis::Y, id_,
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
      : window_(window->window()),
        context_(SDL_GL_CreateContext(window->window())) {
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

  void Swap() { SDL_GL_SwapWindow(window_); }

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

Key::Type ToKey(SDL_Keysym key) {
  switch (key.sym) {
    case SDLK_RETURN:
      return Key::Return;
    case SDLK_ESCAPE:
      return Key::Escape;
    case SDLK_BACKSPACE:
      return Key::Backspace;
    case SDLK_TAB:
      return Key::Tab;
    case SDLK_SPACE:
      return Key::Space;
    case SDLK_EXCLAIM:
      return Key::Exclaim;
    case SDLK_QUOTEDBL:
      return Key::Quotedbl;
    case SDLK_HASH:
      return Key::Hash;
    case SDLK_PERCENT:
      return Key::Percent;
    case SDLK_DOLLAR:
      return Key::Dollar;
    case SDLK_AMPERSAND:
      return Key::Ampersand;
    case SDLK_QUOTE:
      return Key::Quote;
    case SDLK_LEFTPAREN:
      return Key::Leftparen;
    case SDLK_RIGHTPAREN:
      return Key::Rightparen;
    case SDLK_ASTERISK:
      return Key::Asterisk;
    case SDLK_PLUS:
      return Key::Plus;
    case SDLK_COMMA:
      return Key::Comma;
    case SDLK_MINUS:
      return Key::Minus;
    case SDLK_PERIOD:
      return Key::Period;
    case SDLK_SLASH:
      return Key::Slash;
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
    case SDLK_COLON:
      return Key::Colon;
    case SDLK_SEMICOLON:
      return Key::Semicolon;
    case SDLK_LESS:
      return Key::Less;
    case SDLK_EQUALS:
      return Key::Equals;
    case SDLK_GREATER:
      return Key::Greater;
    case SDLK_QUESTION:
      return Key::Question;
    case SDLK_AT:
      return Key::At;
    case SDLK_LEFTBRACKET:
      return Key::Leftbracket;
    case SDLK_BACKSLASH:
      return Key::Backslash;
    case SDLK_RIGHTBRACKET:
      return Key::Rightbracket;
    case SDLK_CARET:
      return Key::Caret;
    case SDLK_UNDERSCORE:
      return Key::Underscore;
    case SDLK_BACKQUOTE:
      return Key::Backquote;
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
    case SDLK_CAPSLOCK:
      return Key::Capslock;
    case SDLK_F1:
      return Key::F1;
    case SDLK_F2:
      return Key::F2;
    case SDLK_F3:
      return Key::F3;
    case SDLK_F4:
      return Key::F4;
    case SDLK_F5:
      return Key::F5;
    case SDLK_F6:
      return Key::F6;
    case SDLK_F7:
      return Key::F7;
    case SDLK_F8:
      return Key::F8;
    case SDLK_F9:
      return Key::F9;
    case SDLK_F10:
      return Key::F10;
    case SDLK_F11:
      return Key::F11;
    case SDLK_F12:
      return Key::F12;
    case SDLK_PRINTSCREEN:
      return Key::Printscreen;
    case SDLK_SCROLLLOCK:
      return Key::Scrolllock;
    case SDLK_PAUSE:
      return Key::Pause;
    case SDLK_INSERT:
      return Key::Insert;
    case SDLK_HOME:
      return Key::Home;
    case SDLK_PAGEUP:
      return Key::Pageup;
    case SDLK_DELETE:
      return Key::Delete;
    case SDLK_END:
      return Key::End;
    case SDLK_PAGEDOWN:
      return Key::Pagedown;
    case SDLK_RIGHT:
      return Key::Right;
    case SDLK_LEFT:
      return Key::Left;
    case SDLK_DOWN:
      return Key::Down;
    case SDLK_UP:
      return Key::Up;
    case SDLK_NUMLOCKCLEAR:
      return Key::Numlockclear;
    case SDLK_KP_DIVIDE:
      return Key::Kp_Divide;
    case SDLK_KP_MULTIPLY:
      return Key::Kp_Multiply;
    case SDLK_KP_MINUS:
      return Key::Kp_Minus;
    case SDLK_KP_PLUS:
      return Key::Kp_Plus;
    case SDLK_KP_ENTER:
      return Key::Kp_Enter;
    case SDLK_KP_1:
      return Key::Kp_1;
    case SDLK_KP_2:
      return Key::Kp_2;
    case SDLK_KP_3:
      return Key::Kp_3;
    case SDLK_KP_4:
      return Key::Kp_4;
    case SDLK_KP_5:
      return Key::Kp_5;
    case SDLK_KP_6:
      return Key::Kp_6;
    case SDLK_KP_7:
      return Key::Kp_7;
    case SDLK_KP_8:
      return Key::Kp_8;
    case SDLK_KP_9:
      return Key::Kp_9;
    case SDLK_KP_0:
      return Key::Kp_0;
    case SDLK_KP_PERIOD:
      return Key::Kp_Period;
    case SDLK_APPLICATION:
      return Key::Application;
    case SDLK_POWER:
      return Key::Power;
    case SDLK_KP_EQUALS:
      return Key::Kp_Equals;
    case SDLK_F13:
      return Key::F13;
    case SDLK_F14:
      return Key::F14;
    case SDLK_F15:
      return Key::F15;
    case SDLK_F16:
      return Key::F16;
    case SDLK_F17:
      return Key::F17;
    case SDLK_F18:
      return Key::F18;
    case SDLK_F19:
      return Key::F19;
    case SDLK_F20:
      return Key::F20;
    case SDLK_F21:
      return Key::F21;
    case SDLK_F22:
      return Key::F22;
    case SDLK_F23:
      return Key::F23;
    case SDLK_F24:
      return Key::F24;
    case SDLK_EXECUTE:
      return Key::Execute;
    case SDLK_HELP:
      return Key::Help;
    case SDLK_MENU:
      return Key::Menu;
    case SDLK_SELECT:
      return Key::Select;
    case SDLK_STOP:
      return Key::Stop;
    case SDLK_AGAIN:
      return Key::Again;
    case SDLK_UNDO:
      return Key::Undo;
    case SDLK_CUT:
      return Key::Cut;
    case SDLK_COPY:
      return Key::Copy;
    case SDLK_PASTE:
      return Key::Paste;
    case SDLK_FIND:
      return Key::Find;
    case SDLK_MUTE:
      return Key::Mute;
    case SDLK_VOLUMEUP:
      return Key::Volumeup;
    case SDLK_VOLUMEDOWN:
      return Key::Volumedown;
    case SDLK_KP_COMMA:
      return Key::Kp_Comma;
    case SDLK_KP_EQUALSAS400:
      return Key::Kp_Equalsas400;
    case SDLK_ALTERASE:
      return Key::Alterase;
    case SDLK_SYSREQ:
      return Key::Sysreq;
    case SDLK_CANCEL:
      return Key::Cancel;
    case SDLK_CLEAR:
      return Key::Clear;
    case SDLK_PRIOR:
      return Key::Prior;
    case SDLK_RETURN2:
      return Key::Return2;
    case SDLK_SEPARATOR:
      return Key::Separator;
    case SDLK_OUT:
      return Key::Out;
    case SDLK_OPER:
      return Key::Oper;
    case SDLK_CLEARAGAIN:
      return Key::Clearagain;
    case SDLK_CRSEL:
      return Key::Crsel;
    case SDLK_EXSEL:
      return Key::Exsel;
    case SDLK_KP_00:
      return Key::Kp_00;
    case SDLK_KP_000:
      return Key::Kp_000;
    case SDLK_THOUSANDSSEPARATOR:
      return Key::Thousandsseparator;
    case SDLK_DECIMALSEPARATOR:
      return Key::Decimalseparator;
    case SDLK_CURRENCYUNIT:
      return Key::Currencyunit;
    case SDLK_CURRENCYSUBUNIT:
      return Key::Currencysubunit;
    case SDLK_KP_LEFTPAREN:
      return Key::Kp_Leftparen;
    case SDLK_KP_RIGHTPAREN:
      return Key::Kp_Rightparen;
    case SDLK_KP_LEFTBRACE:
      return Key::Kp_Leftbrace;
    case SDLK_KP_RIGHTBRACE:
      return Key::Kp_Rightbrace;
    case SDLK_KP_TAB:
      return Key::Kp_Tab;
    case SDLK_KP_BACKSPACE:
      return Key::Kp_Backspace;
    case SDLK_KP_A:
      return Key::Kp_A;
    case SDLK_KP_B:
      return Key::Kp_B;
    case SDLK_KP_C:
      return Key::Kp_C;
    case SDLK_KP_D:
      return Key::Kp_D;
    case SDLK_KP_E:
      return Key::Kp_E;
    case SDLK_KP_F:
      return Key::Kp_F;
    case SDLK_KP_XOR:
      return Key::Kp_Xor;
    case SDLK_KP_POWER:
      return Key::Kp_Power;
    case SDLK_KP_PERCENT:
      return Key::Kp_Percent;
    case SDLK_KP_LESS:
      return Key::Kp_Less;
    case SDLK_KP_GREATER:
      return Key::Kp_Greater;
    case SDLK_KP_AMPERSAND:
      return Key::Kp_Ampersand;
    case SDLK_KP_DBLAMPERSAND:
      return Key::Kp_Dblampersand;
    case SDLK_KP_VERTICALBAR:
      return Key::Kp_Verticalbar;
    case SDLK_KP_DBLVERTICALBAR:
      return Key::Kp_Dblverticalbar;
    case SDLK_KP_COLON:
      return Key::Kp_Colon;
    case SDLK_KP_HASH:
      return Key::Kp_Hash;
    case SDLK_KP_SPACE:
      return Key::Kp_Space;
    case SDLK_KP_AT:
      return Key::Kp_At;
    case SDLK_KP_EXCLAM:
      return Key::Kp_Exclam;
    case SDLK_KP_MEMSTORE:
      return Key::Kp_Memstore;
    case SDLK_KP_MEMRECALL:
      return Key::Kp_Memrecall;
    case SDLK_KP_MEMCLEAR:
      return Key::Kp_Memclear;
    case SDLK_KP_MEMADD:
      return Key::Kp_Memadd;
    case SDLK_KP_MEMSUBTRACT:
      return Key::Kp_Memsubtract;
    case SDLK_KP_MEMMULTIPLY:
      return Key::Kp_Memmultiply;
    case SDLK_KP_MEMDIVIDE:
      return Key::Kp_Memdivide;
    case SDLK_KP_PLUSMINUS:
      return Key::Kp_Plusminus;
    case SDLK_KP_CLEAR:
      return Key::Kp_Clear;
    case SDLK_KP_CLEARENTRY:
      return Key::Kp_Clearentry;
    case SDLK_KP_BINARY:
      return Key::Kp_Binary;
    case SDLK_KP_OCTAL:
      return Key::Kp_Octal;
    case SDLK_KP_DECIMAL:
      return Key::Kp_Decimal;
    case SDLK_KP_HEXADECIMAL:
      return Key::Kp_Hexadecimal;
    case SDLK_LCTRL:
      return Key::Lctrl;
    case SDLK_LSHIFT:
      return Key::Lshift;
    case SDLK_LALT:
      return Key::Lalt;
    case SDLK_LGUI:
      return Key::Lgui;
    case SDLK_RCTRL:
      return Key::Rctrl;
    case SDLK_RSHIFT:
      return Key::Rshift;
    case SDLK_RALT:
      return Key::Ralt;
    case SDLK_RGUI:
      return Key::Rgui;
    case SDLK_MODE:
      return Key::Mode;
    case SDLK_AUDIONEXT:
      return Key::Audionext;
    case SDLK_AUDIOPREV:
      return Key::Audioprev;
    case SDLK_AUDIOSTOP:
      return Key::Audiostop;
    case SDLK_AUDIOPLAY:
      return Key::Audioplay;
    case SDLK_AUDIOMUTE:
      return Key::Audiomute;
    case SDLK_MEDIASELECT:
      return Key::Mediaselect;
    case SDLK_WWW:
      return Key::Www;
    case SDLK_MAIL:
      return Key::Mail;
    case SDLK_CALCULATOR:
      return Key::Calculator;
    case SDLK_COMPUTER:
      return Key::Computer;
    case SDLK_AC_SEARCH:
      return Key::Ac_Search;
    case SDLK_AC_HOME:
      return Key::Ac_Home;
    case SDLK_AC_BACK:
      return Key::Ac_Back;
    case SDLK_AC_FORWARD:
      return Key::Ac_Forward;
    case SDLK_AC_STOP:
      return Key::Ac_Stop;
    case SDLK_AC_REFRESH:
      return Key::Ac_Refresh;
    case SDLK_AC_BOOKMARKS:
      return Key::Ac_Bookmarks;
    case SDLK_BRIGHTNESSDOWN:
      return Key::Brightnessdown;
    case SDLK_BRIGHTNESSUP:
      return Key::Brightnessup;
    case SDLK_DISPLAYSWITCH:
      return Key::Displayswitch;
    case SDLK_KBDILLUMTOGGLE:
      return Key::Kbdillumtoggle;
    case SDLK_KBDILLUMDOWN:
      return Key::Kbdillumdown;
    case SDLK_KBDILLUMUP:
      return Key::Kbdillumup;
    case SDLK_EJECT:
      return Key::Eject;
    case SDLK_SLEEP:
      return Key::Sleep;
    default:
      assert(0 && key.sym && "Invalid keyboard button");
      return Key::Invalid;
  }
}

MouseButton::Type ToKey(SDL_MouseButtonEvent mb) {
  switch (mb.button) {
    case SDL_BUTTON_LEFT:
      return MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
      return MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
      return MouseButton::Right;
    case SDL_BUTTON_X1:
      return MouseButton::X1;
    case SDL_BUTTON_X2:
      return MouseButton::X2;
    default:
      assert(0 && mb.button && "Invalid mouse button");
      return MouseButton::Invalid;
  }
}

int FindOculusDisplay(const VideoDisplays& displays) {
  for (size_t i = 0; i < displays.displays().size(); ++i) {
    DisplayInfo di = displays.displays()[i];
    if (di.name() == "Rift DK1" || di.name() == "Rift DK" ||
        (di.width() == 1280 && di.height() == 720)) {
      return i;
    }
  }
  return -1;
}

int DeterminePrimaryDisplayId(const Settings& settings,
                              const VideoDisplays& displays,
                              bool* foundoculus) {
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
    *foundoculus =
        settings.oculus_vr_detection() == OculusVrDetection::Oculusvr;
    return settings.primary_display_id();
  }
}

int TwEventSDL2(const SDL_Event* event) {
  int handled = 0;
  static int s_KeyMod = 0;

  if (event == NULL) {
    return 0;
  }

  switch (event->type) {
    case SDL_TEXTINPUT:
      if (event->text.text[0] != 0 && event->text.text[1] == 0) {
        if (s_KeyMod & TW_KMOD_CTRL && event->text.text[0] < 32) {
          handled = TwKeyPressed(event->text.text[0] + 'a' - 1, s_KeyMod);
        } else {
          if (s_KeyMod & KMOD_RALT) {
            s_KeyMod &= ~KMOD_CTRL;
          }
          handled = TwKeyPressed(event->text.text[0], s_KeyMod);
        }
      }
      s_KeyMod = 0;
      break;
    case SDL_KEYDOWN:
      if (event->key.keysym.sym & SDLK_SCANCODE_MASK) {
        int key = 0;
        switch (event->key.keysym.sym) {
          case SDLK_UP:
            key = TW_KEY_UP;
            break;
          case SDLK_DOWN:
            key = TW_KEY_DOWN;
            break;
          case SDLK_RIGHT:
            key = TW_KEY_RIGHT;
            break;
          case SDLK_LEFT:
            key = TW_KEY_LEFT;
            break;
          case SDLK_INSERT:
            key = TW_KEY_INSERT;
            break;
          case SDLK_HOME:
            key = TW_KEY_HOME;
            break;
          case SDLK_END:
            key = TW_KEY_END;
            break;
          case SDLK_PAGEUP:
            key = TW_KEY_PAGE_UP;
            break;
          case SDLK_PAGEDOWN:
            key = TW_KEY_PAGE_DOWN;
            break;
          default:
            if (event->key.keysym.sym >= SDLK_F1 &&
                event->key.keysym.sym <= SDLK_F12) {
              key = event->key.keysym.sym + TW_KEY_F1 - SDLK_F1;
            }
        }
        if (key != 0) {
          handled = TwKeyPressed(key, event->key.keysym.mod);
        }
      } else if (event->key.keysym.mod & TW_KMOD_ALT) {
        handled =
            TwKeyPressed(event->key.keysym.sym & 0xFF, event->key.keysym.mod);
      } else {
        s_KeyMod = event->key.keysym.mod;
      }
      break;
    case SDL_KEYUP:
      s_KeyMod = 0;
      break;
    case SDL_MOUSEMOTION:
      handled = TwMouseMotion(event->motion.xrel, event->motion.yrel);
      break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
      if (event->type == SDL_MOUSEBUTTONDOWN &&
          (event->button.button == 4 ||
           event->button.button == 5)) {  // mouse wheel
        static int s_WheelPos = 0;
        if (event->button.button == 4) {
          ++s_WheelPos;
        } else {
          --s_WheelPos;
        }
        handled = TwMouseWheel(s_WheelPos);
      } else {
        handled =
            TwMouseButton((event->type == SDL_MOUSEBUTTONUP) ? TW_MOUSE_RELEASED
                                                             : TW_MOUSE_PRESSED,
                          (TwMouseButtonID)event->button.button);
      }
      break;
  }

  return handled;
}

void logic() {
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
  Settings settings;
  settings.Load();

  Sdl sdl(settings);
  SDL_DisableScreenSaver();
  VideoDisplays displays;

  std::shared_ptr<Window> primaryscreen;
  std::vector<std::shared_ptr<Window>> windows;
  std::vector<std::shared_ptr<BlackRenderer>> blacks;

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

      const int width = isprimaryscrreen == false || settings.width() == 0
                            ? di.width()
                            : settings.width();
      const int height = isprimaryscrreen == false || settings.height() == 0
                             ? di.height()
                             : settings.height();
      if (width > di.width()) {
        throw "Target width is too large";
      }
      if (height > di.height()) {
        throw "height is too large";
      }

      const int x = di.x() + (di.width() - width) / 2;
      const int y = di.y() + (di.height() - height) / 2;
      std::shared_ptr<Window> screen(
          new Window(isprimaryscrreen ? "Game" : "Black", x, y, width, height,
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
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Renders with the oculus vr");
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

  bool inside = false;

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

    const bool lock = inside && game.lock_mouse();
    SDL_SetRelativeMouseMode(lock ? SDL_TRUE : SDL_FALSE);

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        game.Quit();
      } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        const bool down = event.type == SDL_KEYDOWN;
        game.inputsystem().OnKeyboardKey(ToKey(event.key.keysym), down);
      } else if (event.type == SDL_MOUSEBUTTONDOWN ||
                 event.type == SDL_MOUSEBUTTONUP) {
        const bool down = event.type == SDL_MOUSEBUTTONDOWN;
        game.inputsystem().OnMouseButton(ToKey(event.button), down);
      } else if (event.type == SDL_MOUSEMOTION) {
        if (lock && game.istweaking() == false) {
          xrel += event.motion.xrel;
          yrel += event.motion.yrel;
        }
      } else if (event.type == SDL_JOYBUTTONDOWN ||
                 event.type == SDL_JOYBUTTONUP) {
        const bool down = event.type == SDL_JOYBUTTONDOWN;
        game.inputsystem().OnJoystickButton(event.jbutton.button,
                                            event.jbutton.which, down);
      } else if (event.type == SDL_JOYAXISMOTION) {
        game.inputsystem().OnJoystickAxis(event.jaxis.axis, event.jaxis.which,
                                          event.jaxis.value / 32768.0f);
      } else if (event.type == SDL_WINDOWEVENT) {
        const auto mouseEvent = event.window.event;
        if (mouseEvent == SDL_WINDOWEVENT_ENTER ||
            mouseEvent == SDL_WINDOWEVENT_LEAVE) {
          inside = mouseEvent == SDL_WINDOWEVENT_ENTER;
        }
      }

      if (lock && game.istweaking()) {
        TwEventSDL2(&event);
      }
    }
    for (auto js : joysticks) {
      js->SendPov(&game);
    }

    const float size = static_cast<float>(
        std::max(primaryscreen->height(), primaryscreen->width()));
    float dx = xrel / size;
    float dy = yrel / size;
    const float sensitivity = 10.0f;
    game.inputsystem().OnMouseAxis(Axis::X, dx * sensitivity);
    game.inputsystem().OnMouseAxis(Axis::Y, dy * sensitivity);
  }
}

int main(int argc, char* argv[]) {
  try {
    logic();
    return 0;
  }
  catch (...) {
    const std::string message = GrabExceptionInformation();
    Error("Error!", message);
    return 1;
  }
}
