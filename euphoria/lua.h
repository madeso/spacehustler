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
      virtual void Get(lua_State* state) = 0;
  };
}  // namespace internal

/** Gets the lua errors that are detected in the lua engine.
@returns the errors
 */
const std::vector<std::string>& GetGlobalErrors();

class Lua;

/** A lua table.
*/
class Table {
  public:
    /** Constructor.
    @param state the state where to create the table.
     */
    explicit Table(Lua* state);
    ~Table();

    /** Sets a lua variable to value.
    @param name the name of the variable
    @param value the value of the new variable
     */
    void Set(const std::string& name, float value);

    /** Pushes the table to the stack
    @param state the state that contains the stack.
     */
    void PushToState(lua_State* state) const;
  private:
    lua_State* state_;
    int reference_;
};

/** Function call.
Only instantiate once.
 */
class FunctionCall {
  public:
    /** Call a lua function.
    @param state the state to use
    @param name the name of the function
     */
    FunctionCall(lua_State* state, const std::string& name);

    /** Supply a string argument.
    @param str the string
     */
    void Argument(const std::string& str);

    /** Supply a integer argument.
    @param i the int
     */
    void Argument(int i);

    /** Supply a table.
    @param t the table
     */
    void Argument(const Table& t);

    /** Supply a float argument.
    @param f the float
     */
    void Argument(float f);

    /** Supply a light user data.
    @param v the light user data.
     */
    void Argument(void* v);

    /** Get a return string.
    @param str where to store the string.
     */
    void Return(std::string* str);

    /** Get a return int.
    @param i where to store the int.
     */
    void Return(int* i);

    /** Call the function.
     */
    void Call();

  private:
    lua_State* state_;
    int args_;
    std::vector<std::shared_ptr<internal::FunctionReturn>> returns_;
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
    void RunFile(const std::string& filename);

    /** Run code.
    @param code the code to run.
     */
    void RunCode(const std::string& code);

    /** Set a global float.
    @param name the name of the global
    @param value the value of the global
     */
    void SetGlobal(const std::string& name, float value);

    /** Gets the state.
    @returns the state
     */
    lua_State* state();

  private:
    lua_State* state_;
};

#endif  // EUPHORIA_LUA_H_
