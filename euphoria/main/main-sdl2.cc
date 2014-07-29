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
#include "euphoria/str.h"

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
        refresh_rate_(mode.refresh_rate),
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

  int refresh_rate() const {
    assert(this);
    return refresh_rate_;
  }

  const std::string& pixel_format() const {
    assert(this);
    return pixel_format_;
  }

 private:
  int width_;
  int height_;
  int refresh_rate_;
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
      : id_(id), joystick_(SDL_JoystickOpen(id)), num_hats_(0) {
    if (joystick_ == NULL) {
      const std::string error = Str() << "Failed to open joystick at " << id;
      throw error;
    }

    num_hats_ = SDL_JoystickNumHats(joystick_);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Opened joystick %s on %d: #axes: %d /"
                " #buttons: %d / #balls: %d / #hats: %d",
                SDL_JoystickNameForIndex(id_), id_,
                SDL_JoystickNumAxes(joystick_),
                SDL_JoystickNumButtons(joystick_),
                SDL_JoystickNumBalls(joystick_), num_hats_);
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
      for (int i = 0; i < num_hats_; ++i) {
        const Uint8 hat = SDL_JoystickGetHat(joystick_, i);
        const Posi values = GetHatValues(hat);

        game->inputsystem().OnJoystickPov(Axis::X, i, id_,
                                          static_cast<float>(values.first));
        game->inputsystem().OnJoystickPov(Axis::Y, i, id_,
                                          static_cast<float>(values.second));
      }
    }
  }

 private:
  int id_;
  int num_hats_;
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

Key ToKey(SDL_Keysym key) {
  switch (key.sym) {
    case SDLK_RETURN:
      return Key::RETURN;
    case SDLK_ESCAPE:
      return Key::ESCAPE;
    case SDLK_BACKSPACE:
      return Key::BACKSPACE;
    case SDLK_TAB:
      return Key::TAB;
    case SDLK_SPACE:
      return Key::SPACE;
    case SDLK_EXCLAIM:
      return Key::EXCLAIM;
    case SDLK_QUOTEDBL:
      return Key::QUOTEDBL;
    case SDLK_HASH:
      return Key::HASH;
    case SDLK_PERCENT:
      return Key::PERCENT;
    case SDLK_DOLLAR:
      return Key::DOLLAR;
    case SDLK_AMPERSAND:
      return Key::AMPERSAND;
    case SDLK_QUOTE:
      return Key::QUOTE;
    case SDLK_LEFTPAREN:
      return Key::LEFT_PAREN;
    case SDLK_RIGHTPAREN:
      return Key::RIGHT_PAREN;
    case SDLK_ASTERISK:
      return Key::ASTERIX;
    case SDLK_PLUS:
      return Key::PLUS;
    case SDLK_COMMA:
      return Key::COMMA;
    case SDLK_MINUS:
      return Key::MINUS;
    case SDLK_PERIOD:
      return Key::PERIOD;
    case SDLK_SLASH:
      return Key::SLASH;
    case SDLK_0:
      return Key::NUM_0;
    case SDLK_1:
      return Key::NUM_1;
    case SDLK_2:
      return Key::NUM_2;
    case SDLK_3:
      return Key::NUM_3;
    case SDLK_4:
      return Key::NUM_4;
    case SDLK_5:
      return Key::NUM_5;
    case SDLK_6:
      return Key::NUM_6;
    case SDLK_7:
      return Key::NUM_7;
    case SDLK_8:
      return Key::NUM_8;
    case SDLK_9:
      return Key::NUM_9;
    case SDLK_COLON:
      return Key::COLON;
    case SDLK_SEMICOLON:
      return Key::SEMICOLON;
    case SDLK_LESS:
      return Key::LESS;
    case SDLK_EQUALS:
      return Key::EQUALS;
    case SDLK_GREATER:
      return Key::GREATER;
    case SDLK_QUESTION:
      return Key::QUESTION;
    case SDLK_AT:
      return Key::AT;
    case SDLK_LEFTBRACKET:
      return Key::LEFT_BRACKET;
    case SDLK_BACKSLASH:
      return Key::BACKSLASH;
    case SDLK_RIGHTBRACKET:
      return Key::RIGHT_BRACKET;
    case SDLK_CARET:
      return Key::CARET;
    case SDLK_UNDERSCORE:
      return Key::UNDERSCORE;
    case SDLK_BACKQUOTE:
      return Key::BACKQUOTE;
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
      return Key::CAPSLOCK;
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
      return Key::PRINTSCREEN;
    case SDLK_SCROLLLOCK:
      return Key::SCROLLLOCK;
    case SDLK_PAUSE:
      return Key::PAUSE;
    case SDLK_INSERT:
      return Key::INSERT;
    case SDLK_HOME:
      return Key::HOME;
    case SDLK_PAGEUP:
      return Key::PAGEUP;
    case SDLK_DELETE:
      return Key::DELETE;
    case SDLK_END:
      return Key::END;
    case SDLK_PAGEDOWN:
      return Key::PAGEDOWN;
    case SDLK_RIGHT:
      return Key::RIGHT;
    case SDLK_LEFT:
      return Key::LEFT;
    case SDLK_DOWN:
      return Key::DOWN;
    case SDLK_UP:
      return Key::UP;
    case SDLK_NUMLOCKCLEAR:
      return Key::NUMLOCK_CLEAR;
    case SDLK_KP_DIVIDE:
      return Key::KEYPAD_DIVIDE;
    case SDLK_KP_MULTIPLY:
      return Key::KEYPAD_MULTIPLY;
    case SDLK_KP_MINUS:
      return Key::KEYPAD_MINUS;
    case SDLK_KP_PLUS:
      return Key::KEYPAD_PLUS;
    case SDLK_KP_ENTER:
      return Key::KEYPAD_ENTER;
    case SDLK_KP_1:
      return Key::KEYPAD_1;
    case SDLK_KP_2:
      return Key::KEYPAD_2;
    case SDLK_KP_3:
      return Key::KEYPAD_3;
    case SDLK_KP_4:
      return Key::KEYPAD_4;
    case SDLK_KP_5:
      return Key::KEYPAD_5;
    case SDLK_KP_6:
      return Key::KEYPAD_6;
    case SDLK_KP_7:
      return Key::KEYPAD_7;
    case SDLK_KP_8:
      return Key::KEYPAD_8;
    case SDLK_KP_9:
      return Key::KEYPAD_9;
    case SDLK_KP_0:
      return Key::KEYPAD_0;
    case SDLK_KP_PERIOD:
      return Key::KEYPAD_PERIOD;
    case SDLK_APPLICATION:
      return Key::APPLICATION;
    case SDLK_POWER:
      return Key::POWER;
    case SDLK_KP_EQUALS:
      return Key::KEYPAD_EQUALS;
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
      return Key::EXECUTE;
    case SDLK_HELP:
      return Key::HELP;
    case SDLK_MENU:
      return Key::MENU;
    case SDLK_SELECT:
      return Key::SELECT;
    case SDLK_STOP:
      return Key::STOP;
    case SDLK_AGAIN:
      return Key::AGAIN;
    case SDLK_UNDO:
      return Key::UNDO;
    case SDLK_CUT:
      return Key::CUT;
    case SDLK_COPY:
      return Key::COPY;
    case SDLK_PASTE:
      return Key::PASTE;
    case SDLK_FIND:
      return Key::FIND;
    case SDLK_MUTE:
      return Key::MUTE;
    case SDLK_VOLUMEUP:
      return Key::VOLUME_UP;
    case SDLK_VOLUMEDOWN:
      return Key::VOLUME_DOWN;
    case SDLK_KP_COMMA:
      return Key::KEYPAD_COMMA;
    case SDLK_KP_EQUALSAS400:
      return Key::KEYPAD_EQUALS_AS_400;
    case SDLK_ALTERASE:
      return Key::ALT_ERASE;
    case SDLK_SYSREQ:
      return Key::SYSREQ;
    case SDLK_CANCEL:
      return Key::CANCEL;
    case SDLK_CLEAR:
      return Key::CLEAR;
    case SDLK_PRIOR:
      return Key::PRIOR;
    case SDLK_RETURN2:
      return Key::SECOND_RETURN;
    case SDLK_SEPARATOR:
      return Key::SEPARATOR;
    case SDLK_OUT:
      return Key::OUT;
    case SDLK_OPER:
      return Key::OPER;
    case SDLK_CLEARAGAIN:
      return Key::CLEAR_AGAIN;
    case SDLK_CRSEL:
      return Key::CRSEL;
    case SDLK_EXSEL:
      return Key::EXSEL;
    case SDLK_KP_00:
      return Key::KEYPAD_00;
    case SDLK_KP_000:
      return Key::KEYPAD_000;
    case SDLK_THOUSANDSSEPARATOR:
      return Key::THOUSANDSEPARATOR;
    case SDLK_DECIMALSEPARATOR:
      return Key::DECIMALSEPARATOR;
    case SDLK_CURRENCYUNIT:
      return Key::CURRENCY_UNIT;
    case SDLK_CURRENCYSUBUNIT:
      return Key::CURRENCY_SUBUNIT;
    case SDLK_KP_LEFTPAREN:
      return Key::KEYPAD_LEFTPAREN;
    case SDLK_KP_RIGHTPAREN:
      return Key::KEYPAD_RIGHTPAREN;
    case SDLK_KP_LEFTBRACE:
      return Key::KEYPAD_LEFTBRACE;
    case SDLK_KP_RIGHTBRACE:
      return Key::KEYPAD_RIGHTBRACE;
    case SDLK_KP_TAB:
      return Key::KEYPAD_TAB;
    case SDLK_KP_BACKSPACE:
      return Key::KEYPAD_BACKSPACE;
    case SDLK_KP_A:
      return Key::KEYPAD_A;
    case SDLK_KP_B:
      return Key::KEYPAD_B;
    case SDLK_KP_C:
      return Key::KEYPAD_C;
    case SDLK_KP_D:
      return Key::KEYPAD_D;
    case SDLK_KP_E:
      return Key::KEYPAD_E;
    case SDLK_KP_F:
      return Key::KEYPAD_F;
    case SDLK_KP_XOR:
      return Key::KEYPAD_XOR;
    case SDLK_KP_POWER:
      return Key::KEYPAD_POWER;
    case SDLK_KP_PERCENT:
      return Key::KEYPAD_PERCENT;
    case SDLK_KP_LESS:
      return Key::KEYPAD_LESS;
    case SDLK_KP_GREATER:
      return Key::KEYPAD_GREATER;
    case SDLK_KP_AMPERSAND:
      return Key::KEYPAD_AMPERSAND;
    case SDLK_KP_DBLAMPERSAND:
      return Key::KEYPAD_DOUBLE_AMPERSAND;
    case SDLK_KP_VERTICALBAR:
      return Key::KEYPAD_VERTICAL_BAR;
    case SDLK_KP_DBLVERTICALBAR:
      return Key::KEYPAD_DOUBLE_VERTICLE_BAR;
    case SDLK_KP_COLON:
      return Key::KEYPAD_COLON;
    case SDLK_KP_HASH:
      return Key::KEYPAD_HASH;
    case SDLK_KP_SPACE:
      return Key::KEYPAD_SPACE;
    case SDLK_KP_AT:
      return Key::KEYPAD_AT;
    case SDLK_KP_EXCLAM:
      return Key::KEYPAD_EXCLAM;
    case SDLK_KP_MEMSTORE:
      return Key::KEYPAD_MEM_STORE;
    case SDLK_KP_MEMRECALL:
      return Key::KEYPAD_MEM_RECALL;
    case SDLK_KP_MEMCLEAR:
      return Key::KEYPAD_MEM_CLEAR;
    case SDLK_KP_MEMADD:
      return Key::KEYPAD_MEM_ADD;
    case SDLK_KP_MEMSUBTRACT:
      return Key::KEYPAD_MEM_SUBTRACT;
    case SDLK_KP_MEMMULTIPLY:
      return Key::KEYPAD_MEM_MULTIPLY;
    case SDLK_KP_MEMDIVIDE:
      return Key::KEYPAD_MEM_DIVIDE;
    case SDLK_KP_PLUSMINUS:
      return Key::KEYPAD_PLUS_MINUS;
    case SDLK_KP_CLEAR:
      return Key::KEYPAD_CLEAR;
    case SDLK_KP_CLEARENTRY:
      return Key::KEYPAD_CLEAR_ENTRY;
    case SDLK_KP_BINARY:
      return Key::KEYPAD_BINARY;
    case SDLK_KP_OCTAL:
      return Key::KEYPAD_OCTAL;
    case SDLK_KP_DECIMAL:
      return Key::KEYPAD_DECIMAL;
    case SDLK_KP_HEXADECIMAL:
      return Key::KEYPAD_HEXADECIMAL;
    case SDLK_LCTRL:
      return Key::CTRL_LEFT;
    case SDLK_LSHIFT:
      return Key::SHIFT_LEFT;
    case SDLK_LALT:
      return Key::ALT_LEFT;
    case SDLK_LGUI:
      return Key::GUI_LEFT;
    case SDLK_RCTRL:
      return Key::CTRL_RIGHT;
    case SDLK_RSHIFT:
      return Key::SHIFT_RIGHT;
    case SDLK_RALT:
      return Key::ALT_RIGHT;
    case SDLK_RGUI:
      return Key::GUI_RIGHT;
    case SDLK_MODE:
      return Key::MODE;
    case SDLK_AUDIONEXT:
      return Key::AUDIO_NEXT;
    case SDLK_AUDIOPREV:
      return Key::AUDIO_PREV;
    case SDLK_AUDIOSTOP:
      return Key::AUDIO_STOP;
    case SDLK_AUDIOPLAY:
      return Key::AUDIO_PLAY;
    case SDLK_AUDIOMUTE:
      return Key::AUDIO_MUTE;
    case SDLK_MEDIASELECT:
      return Key::Mediaselect;
    case SDLK_WWW:
      return Key::WWW;
    case SDLK_MAIL:
      return Key::MAIL;
    case SDLK_CALCULATOR:
      return Key::CALCULATOR;
    case SDLK_COMPUTER:
      return Key::COMPUTER;
    case SDLK_AC_SEARCH:
      return Key::AC_SEARCH;
    case SDLK_AC_HOME:
      return Key::AC_HOME;
    case SDLK_AC_BACK:
      return Key::AC_BACK;
    case SDLK_AC_FORWARD:
      return Key::AC_FORWARD;
    case SDLK_AC_STOP:
      return Key::AC_STOP;
    case SDLK_AC_REFRESH:
      return Key::AC_REFRESH;
    case SDLK_AC_BOOKMARKS:
      return Key::AC_BOOKMARKS;
    case SDLK_BRIGHTNESSDOWN:
      return Key::BRIGHTNESS_DOWN;
    case SDLK_BRIGHTNESSUP:
      return Key::BRIGHTNESS_UP;
    case SDLK_DISPLAYSWITCH:
      return Key::DISPLAYSWITCH;
    case SDLK_KBDILLUMTOGGLE:
      return Key::KEYBOARD_ILLUM_TOGGLE;
    case SDLK_KBDILLUMDOWN:
      return Key::KEYBOARD_ILLUM_DOWN;
    case SDLK_KBDILLUMUP:
      return Key::KEYBOARD_ILLUM_UP;
    case SDLK_EJECT:
      return Key::EJECT;
    case SDLK_SLEEP:
      return Key::SLEEP;
    default:
      assert(0 && key.sym && "Invalid keyboard button");
      return Key::INVALID;
  }
}

MouseButton ToKey(SDL_MouseButtonEvent mb) {
  switch (mb.button) {
    case SDL_BUTTON_LEFT:
      return MouseButton::LEFT;
    case SDL_BUTTON_MIDDLE:
      return MouseButton::MIDDLE;
    case SDL_BUTTON_RIGHT:
      return MouseButton::RIGHT;
    case SDL_BUTTON_X1:
      return MouseButton::X1;
    case SDL_BUTTON_X2:
      return MouseButton::X2;
    default:
      assert(0 && mb.button && "Invalid mouse button");
      return MouseButton::INVALID;
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
  if (settings.oculus_vr_detection() == OculusVrDetection::AUTO) {
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
        settings.oculus_vr_detection() == OculusVrDetection::OCULUS_VR;
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
        const std::string error = Str() << "Target width is too large "
          << width << " vs " << di.width();
        throw error;
      }
      if (height > di.height()) {
        const std::string error = Str() << "height is too large "
          << height << " vs " << di.height();
        throw error;
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
