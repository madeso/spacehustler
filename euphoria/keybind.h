// Euphoria - Copyright (c) Gustav

/** @file
Key binding related code
 */

#ifndef EUPHORIA_KEYBIND_H_
#define EUPHORIA_KEYBIND_H_

#include <string>
#include <vector>
#include "euphoria/key.h"
#include "euphoria/lua.h"

/** A single keybind.
 */
class Keybind {
  public:
    /** Constructor.
    @param name the name of the keybind.
    @param var the variable to bind the key to
    @param key the actual key
     */
    Keybind(const std::string& name, const std::string& var,
            const Key::Type key);

    /** Get the name of the lua variable to bind to.
    @returns the name of the lua variable
     */
    const std::string var() const;

    /** Get the actual key of the bind.
    @returns the key
     */
    const Key::Type key() const;
  private:
    std::string name_;
    std::string var_;
    Key::Type key_;
};

/** A list of keybinds.
 */
class KeybindList {
  public:
    /** Constructor.
    @param lua the lua instance twhere the bins will occur
     */
    explicit KeybindList(Lua* lua);

    /** Load keybinds from a file.
    @param filename the filename where to load from
     */
    void Load(const std::string& filename);

    /** React on press or release of a key.
    @param key the key
    @param down true if the key was just pressed down, false if got released
     */
    void OnKey(Key::Type key, bool down);

    /** React on mouse move.
    @param dx the relative mouse movement in x
    @param dy the relative mouse movement in y
    @todo move to a axis binder instead
     */
    void OnMouse(float dx, float dy);

  private:
    Lua* lua_;
    std::vector<Keybind> keys_;
};

#endif  // EUPHORIA_KEYBIND_H_
