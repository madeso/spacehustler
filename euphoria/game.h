  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Game related code.
 */

#ifndef EUPHORIA_GAME_H_
#define EUPHORIA_GAME_H_

#include <memory>
#include <string>

#include "euphoria/keybind.h"

#include "euphoria-config.h" // NOLINT this is the default way to include cmake files

class OglDebug;
class TextureCache;
class ShaderCache;
class World;
class Lua;
class ActionMap;
class Camera;
class SystemContainer;
class EntityList;
class Settings;
class OculusVr;

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

    /** React on state change of a key.
    @param key the key
    @param device the device
    @param state the state
    @see KeybindList::OnKey()
     */
    void OnKey(Key::Type key, int device, float state);

    /** React on axis
    @param axis the axis
    @param device the device
    @param state the new state
    @see KeybindList::OnAxis()
     */
    void OnAxis(Axis::Type axis, int device, float state);

    /** Tests if tweaking is enabled or not.
    @returns true if tweaking is enabled, false if not
     */
    bool istweaking() const;

    /** Quit the game.
    Sets the internal run variable to false.
     */
    void Quit();

  private:
    void SubRender(const Camera& camera);
    bool keep_running_;
    std::unique_ptr<OglDebug> ogldebug_;
    std::unique_ptr<TextureCache> texturecache_;
    std::unique_ptr<ShaderCache> shadercache_;
    std::unique_ptr<World> world_;
    std::unique_ptr<Lua> script_;
    std::unique_ptr<ActionMap> actions_;
    std::unique_ptr<KeybindList> keybinds_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<SystemContainer> container_;
    std::unique_ptr<EntityList> entities_;
    std::unique_ptr<OculusVr> oculusvr_;
    bool istweaking_;
    bool renderoculus_;
#ifdef USE_TWEAKABLES
    std::unique_ptr<TweakerStore> tweakers_;
#endif
};

#endif  // EUPHORIA_GAME_H_
