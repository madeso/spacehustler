// Euphoria - Copyright (c) Gustav

/** @file
Game related code.
 */

#ifndef EUPHORIA_GAME_H_
#define EUPHORIA_GAME_H_

#include <memory>
#include <string>

#include "euphoria/input-system.h"

#include "euphoria-config.h"  // NOLINT this is the default way to include cmake files
#include "euphoria/graphics/camera.h"

namespace euphoria {

class TextureCache;
class ShaderCache;
class World;
class Lua;
class InputAction;
class SystemContainer;
class EntityList;
class Settings;
class OculusVr;
class Fbo;
class Quad;
class Program;

#ifdef USE_TWEAKABLES
class TweakRenderer;
class TweakerStore;
#endif

namespace ui {
class Display;
}  // namespace ui

/** Contains common game data.
 */
class Game {
 public:
  /** Constructor.
  @param settings the settings
  @param renderoculus true if to render oculus ready data
   */
  explicit Game(const Settings& settings);
  ~Game();

  /** Returns if the game should keep running.
  @returns true if it should keep running, false if not.
   */
  bool keep_running() const;

  /** Render the game.
   */
  void Render();

  /** Update the game.
  @param dt the elapsed time in seconds
   */
  void Update(float dt);

  /** Tests if tweaking is enabled or not.
  @returns true if tweaking is enabled, false if not
   */
  bool istweaking() const;

  /** Gets if the mouse is to be locked or not.
  @returns if the mouse is to be locked or not
   */
  bool lock_mouse() const;

  /** Quit the game.
  Sets the internal run variable to false.
   */
  void Quit();

  /** Send a keyboard event.
  @param key the key to act upon
  @param down true if it is down, false if not
   */
  void InputOnKeyboardKey(Key key, bool down);

  /** Send a mouse axis.
  @param axis the axis
  @param value the value
   */
  void InputOnMouseAxis(Axis axis, float value);

  /** Send a mouse button.
  @param button the button to act upon
  @param down true if it is down, false if not
   */
  void InputOnMouseButton(MouseButton button, bool down);

  /** Send a joystick pov.
  @param type the pov type
  @param hat the joystick pov hat
  @param joystick the joystick id
  @param value the value
   */
  void InputOnJoystickPov(Axis type, int hat, int joystick, float value);

  /** Send a joystick button.
  @param button the button to act upon
  @param joystick the joystick id
  @param down true if it is down, false if not
   */
  void InputOnJoystickButton(int button, int joystick, bool down);

  /** Send a joystick axis.
  @param axis the axis
  @param joystick the joystick id
  @param value the value
   */
  void InputOnJoystickAxis(int axis, int joystick, float value);

  OculusVr& oculusvr();

 private:
  void SubRender(const Camera& camera);

 private:
  int width_;
  int height_;
  bool keep_running_;
  std::shared_ptr<input::GlobalToggle> tweakaction_;
  bool last_tweak_action_;
  bool lock_mouse_;
  bool istweaking_;
  input::InputSystem inputsystem_;
  std::unique_ptr<TextureCache> texturecache_;
  std::unique_ptr<ShaderCache> shadercache_;
  std::unique_ptr<World> world_;
  std::unique_ptr<Lua> script_;
  Camera camera_;
  std::unique_ptr<SystemContainer> container_;
  std::unique_ptr<EntityList> entities_;
  std::unique_ptr<OculusVr> oculusvr_;
  std::unique_ptr<ui::Display> hud_;
#ifdef USE_TWEAKABLES
  std::unique_ptr<TweakRenderer> tweak_renderer_;
  std::unique_ptr<TweakerStore> tweak_store_;
#endif
};

}  // namespace euphoria

#endif  // EUPHORIA_GAME_H_
