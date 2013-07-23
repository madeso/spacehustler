// Euphoria - Copyright (c) Gustav

/** @file
Lua related code.
 */

#ifndef EUPHORIA_LUA_H_
#define EUPHORIA_LUA_H_

#include <string>
#include <vector>
#include <memory>

extern "C" {
  struct lua_State;
};

namespace internal {
  /** Function return from lua.
   */
  class FunctionReturn {
    public:
      /** Destructor.
       */
      virtual ~FunctionReturn();

      /** Get the stored lua value.
      @param state the state to get it from.
       */
      virtual void get(lua_State* state) = 0;
  };
}  // namespace internal

/** Function call.
 */
class FunctionCall {
  public:
    /** Call a lua function.
    @param astate the state to use
    @param name the name of the function
     */
    FunctionCall(lua_State* astate, const std::string& name);

    /** Supply a string argument.
    @param str the string
     */
    void arg(const std::string& str);

    /** Supply a integer argument.
    @param i the int
     */
    void arg(int i);

    /** Supply a float argument.
    @param f the float
     */
    void arg(float f);

    /** Supply a light user data.
    @param v the light user data.
     */
    void arg(void* v);

    /** Get a return string.
    @param str where to store the string.
     */
    void ret(std::string* str);

    /** Get a return int.
    @param i where to store the int.
     */
    void ret(int* i);

    /** Call the function.
     */
    void call();

  private:
    lua_State* state;
    int args;
    std::vector<std::shared_ptr<internal::FunctionReturn>> returns;
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

  private:
    lua_State* state;
};

#endif  // EUPHORIA_LUA_H_
