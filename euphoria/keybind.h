// Euphoria - Copyright (c) Gustav

/** @file
Key binding related code
 */

#ifndef EUPHORIA_KEYBIND_H_
#define EUPHORIA_KEYBIND_H_

#include <string>
#include <vector>
#include <map>
#include "euphoria/key.h"
#include "euphoria/lua.h"

/** A action that can be made in game.
 */
class Action {
  public:
    /** constructor.
    @param name the name of the action
    @param var the name of the script variable
    @param lua the lua interpreter
     */
    Action(Lua* lua, const std::string& name, const std::string& var);

    /** Gets the state.
    @returns the state.
     */
    float state() const;

    /** Sets the state.
    @param state the state
     */
    void set_state(float state);
  private:
    Lua* lua_;
    const std::string name_;
    const std::string var_;
    float state_;
};

/** A collection of the valid actions.
 */
class ActionMap {
  public:
    /** Load actions from a file.
    @param filename the the to load from.
    @param lua the lua interpreter to use
     */
    ActionMap(const std::string& filename, Lua* lua);

    /** Get a action from the collection.
    @param actionname the name of the action.
    @return the found action
     */
    Action* const getAction(const std::string& actionname);
  private:
    std::map<std::string, Action> actions_;
};

/** A single key bind.
 */
class Keybind {
  public:
    /** Constructor.
    @param action the action to perform
    @param key the key to react to
    @param device the device the key belongs to
     */
    Keybind(Action* const action, const Key::Type key, int device);

    /** Get the actual key of the bind.
    @returns the key
     */
    const Key::Type key() const;

    /** Get the device the key belongs to.
    @returns the device
     */
    const int device() const;

    /** The the state of the action this key bind is bound to.
    @param state the new state
     */
    void setState(float state);

  private:
    Action* const action_;
    Key::Type key_;
    int device_;
};

/** A list of key binds.
 */
class KeybindList {
  public:
    /** Constructor.
     */
    KeybindList();

    /** Load key binds from a file.
    @param actions the action map
    @param filename the filename where to load from
     */
    void Load(ActionMap* actions, const std::string& filename);

    /** React on press or release of a key.
    @param key the key
    @param device the device
    @param down true if the key was just pressed down, false if got released
     */
    void OnKey(Key::Type key, int device, bool down);

    /** React on state change of a key.
    @param key the key
    @param device the device
    @param state the state
     */
    void OnKey(Key::Type key, int device, float state);

    /** React on mouse move.
    @param dx the relative mouse movement in x
    @param dy the relative mouse movement in y
    @todo move to a axis binder instead
     */
    void OnMouse(float dx, float dy);

  private:
    std::vector<Keybind> keys_;
};

#endif  // EUPHORIA_KEYBIND_H_
