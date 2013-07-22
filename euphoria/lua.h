// Euphoria - Copyright (c) Gustav

/** @file
Lua related code.
 */

#ifndef EUPHORIA_LUA_H_
#define EUPHORIA_LUA_H_

#include <string>

extern "C" {
  struct lua_State;
};

/** Util class for creating and destroying a lua state.
 */
class Lua {
  public:
    /** Creates a lua state.
     */
    Lua();

    /** Destroys the lua state.
     */
    ~Lua();

    /** Add the standard libraries.
     */
    void addStandardLibraries();

    /** Run file.
    @param filename the file to run.
     */
    void runFile(const std::string& filename);

    /** Run code.
    @param code the code to run.
     */
    void runCode(const std::string& code);

    /** Gets the state.
    @returns the state
     */
    lua_State* getState();

  protected:
    /** If there is a error, throw.
    @param errorcode the error code.
     */
    void throwIfError(int errorcode);

  private:
    lua_State* state;
};

#endif  // EUPHORIA_LUA_H_
