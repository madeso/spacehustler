// Euphoria - Copyright (c) Gustav

/** @file
Script embedding file.
 */

#ifndef EUPHORIA_SCRIPT_H_
#define EUPHORIA_SCRIPT_H_

#include <cassert>
#include <string>
#include <map>
#include <memory>
#include <vector>

extern "C" {
  struct lua_State;

  /** External lua func typedef.
  @param L the state.
   */
  typedef int (*lua_CFunction)(lua_State* L);
};

namespace internal {
  /** Internal class to handle scripting arguments.
   */
  class ScriptArgument {
    public:
      /** Destructor.
       */
      virtual ~ScriptArgument();

      /** Returns true if the argument is valid.
      @param state the lua state
      @param position the position of this argument in the stack
      @returns true if the argument is valid, false if not.
       */
      virtual bool isValid(lua_State* state, int position) = 0;

      /** Get the argument to the provided member.
      @param state the lua state
      @param position the position of this argument in the stack
      */
      virtual void get(lua_State* state, int position) = 0;
  };
}  // namespace internal

/** This class helps with overloading script functions.
A script function can have many overloads, it's up to the C++ code to handle
this and this class helps with that.
@see ScriptParams
@see REGISTER_SCRIPT_FUNCTION
 */
class ScriptOverload {
  public:
    /** Constructor.
     */
    ScriptOverload();

    /** Define a light user data argument.
    @param userdata the userdata.
     */
    void define(void** userdata);

    /** Define a float argument.
    @param f the float.
     */
    void define(float* f);

    /** Checks to see if this overload was the one used.
    @returns true if it is the one used, false otherwise.
    @see ScriptParams::fill()
    @see isValid
     */
    operator bool();

    /** Checks to see if this overload was the one used.
    @returns true if it is the one used, false otherwise.
    @see ScriptParams::fill()
     */
    bool isValid();

    /** Internal.
    Validates the function and if it is valid, it get the value from lua and
    sets all the arguments.
    @param argcount the number of arguments
    @param state the lua state
    @return true if valid, false if not.
     */
    bool validate(int argcount, lua_State* state);

  private:
    std::vector<std::shared_ptr<internal::ScriptArgument>> arguments;
    bool valid;
};

/** Encapsulated utility class for getting arguments and returning values.
 */
class ScriptParams {
  public:
    /** Not explicitly called by the user. The user should use the
    REGISTERS_SCRIPT_FUNCTION macro.
    @param astate the state
     */
    explicit ScriptParams(lua_State* astate);

    /** Add a overload to the parameters. At least one overload must be added,
    even it it's a empty one.
    @param overload the overload to add
     */
    void overload(ScriptOverload* overload);

    /** Determine the overload to use or abort if no one can be found.
     */
    void fill();

    /** Return some light userdata.
    @param userdata the light userdata to return
     */
    void returnvar(void* userdata);

    /** Internal. Gets the number of return values that has been added.
    @returns the return count.
     */
    int getReturnCount();

  private:
    std::vector<ScriptOverload*> overloads;
    lua_State* state;
    int retcount;
};

/** Utility class for registring script functions.
 */
class ScriptRegister {
  public:
    /** Constructor.
     */
    ScriptRegister();

    /** Add a function.
    @param name the name of the function.
    @param func the function.
     */
    void add(const std::string& name, lua_CFunction func);

    /** Register all functions on a state.
     */
    void registerAll(lua_State* state);
  private:
    std::map<std::string, lua_CFunction> functions;
};

/** Get the global script registrer.
@returns a pointer to the global script register
 */
ScriptRegister* GetGlobalScriptRegister();

/** Register a script function. The function to add should have the following
structure:
void MyFunc(ScriptParams* params){ ]
@param name the name of the function surrounded by quotes like "MyFunc"
@param func the function to register
 */
#define REGISTER_SCRIPT_FUNCTION(name, func) \
  int Lua_callback_for_##func(lua_State* state) { \
    ScriptParams params(state);\
    func(&params);\
    return params.getReturnCount();\
  }\
  class Lua_class_register_ ## func { \
    public:\
      Lua_class_register_##func() { \
        assert(GetGlobalScriptRegister());\
        GetGlobalScriptRegister()->add(name, Lua_callback_for_##func);\
      }\
  } Lua_static_var_register_ ## func

#endif  // EUPHORIA_SCRIPT_H_
