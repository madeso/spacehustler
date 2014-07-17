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

class OglDebug;
class TextureCache;
class ShaderCache;
class World;
class Lua;
class InputAction;
class Camera;
class SystemContainer;
class EntityList;
class Settings;
class OculusVr;
class Fbo;
class Quad;
class Program;

#ifdef USE_TWEAKABLES
class TweakerStore;
#endif

/** Contains common game data.
 */
class Game {
 public:
  /** Constructor.
  @param settings the settings
  @param renderoculus true if to render oculus ready data
   */
  Game(const Settings& settings, bool renderoculus);
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

  /** Gets the input system.
  @returns the input system
   */
  input::InputSystem& inputsystem();

 private:
  int width_;
  int height_;
  bool keep_running_;
  std::shared_ptr<input::GlobalToggle> tweakaction_;
  bool last_tweak_action_;
  bool lock_mouse_;
  bool istweaking_;
  bool renderoculus_;
  input::InputSystem inputsystem_;
  std::unique_ptr<OglDebug> ogldebug_;
  std::unique_ptr<TextureCache> texturecache_;
  std::unique_ptr<ShaderCache> shadercache_;
  std::unique_ptr<World> world_;
  std::unique_ptr<Lua> script_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<SystemContainer> container_;
  std::unique_ptr<EntityList> entities_;
  std::unique_ptr<OculusVr> oculusvr_;
  std::unique_ptr<Fbo> eyefbo_;
  std::shared_ptr<Program> eyeprogram_;
  std::unique_ptr<Quad> eyequad_;
#ifdef USE_TWEAKABLES
  std::unique_ptr<TweakerStore> tweakers_;
#endif
};

#endif  // EUPHORIA_GAME_H_
