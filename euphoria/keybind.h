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
    @param aname the name of the keybind.
    @param avar the variable to bind the key to
    @param akey the actual key
     */
    Keybind(const std::string& aname, const std::string& avar,
            const Key::Type akey);

    /** Get the name of the lua variable to bind to.
    @returns the name of the lua variable
     */
    const std::string getLuaVar() const;

    /** Get the actual key of the bind.
    @returns the key
     */
    const Key::Type getKey() const;
  private:
    std::string name;
    std::string var;
    Key::Type key;
};

/** A list of keybinds.
 */
class KeybindList {
  public:
    /** Constructor.
    @param alua the lua instance twhere the bins will occur
     */
    explicit KeybindList(Lua* alua);

    /** Load keybinds from a file.
    @param filename the filename where to load from
     */
    void load(const std::string& filename);

    /** React on press or release of a key.
    @param key the key
    @param down true if the key was just pressed down, false if got released
     */
    void onKey(Key::Type key, bool down);
  private:
    Lua* lua;
    std::vector<Keybind> keys;
};

#endif  // EUPHORIA_KEYBIND_H_
