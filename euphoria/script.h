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

      /** Generates a string representation of the argument type.
       */
      virtual std::string toString() const = 0;
  };

  /** Class representing a argument for light user data.
   */
  class LightUserDataScriptArgument : public internal::ScriptArgument {
    public:
      /** Constructor.
      @param adata the data
       */
      explicit LightUserDataScriptArgument(void** adata);
      bool isValid(lua_State* state, int position);
      void get(lua_State* state, int position);
      std::string toString() const;
    private:
      void** data;
  };

  /** Class representing a argument for full user data.
   */
  class FullUserDataScriptArgument : public internal::ScriptArgument {
    public:
      /** Constructor.
      @param adata the data
      @param aname the name of the data
       */
      FullUserDataScriptArgument(void** adata, const std::string& aname);
      bool isValid(lua_State* state, int position);
      void get(lua_State* state, int position);
      std::string toString() const;
    private:
      void** data;
      std::string name;
  };

  /** Called in a exception handler as a return code when everything has failed.
  @return the error code, but it never returns.
  @param name the name of the lua/C function
  @param state the lua state
   */
  int HandleLuaException(const std::string& name, lua_State* state);
}  // namespace internal

class ScriptParams;

/** This class helps with overloading script functions.
A script function can have many overloads, it's up to the C++ code to handle
this and this class helps with that.
@see ScriptParams
@see REGISTER_SCRIPT_FUNCTION
 */
class ScriptOverload {
  public:
    /** Constructor.
    @param aparams the parameters.
     */
    explicit ScriptOverload(ScriptParams* aparams);

    /** Checks to see if this overload was the one used.
    @returns true if it is the one used, false otherwise.
    @see ScriptParams::fill()
     */
    operator bool();

    /** Add a custom script argument.
    @param arg the argument
    @return this
     */
    ScriptOverload& operator<<(std::shared_ptr<internal::ScriptArgument> arg);

    /** Add a float argument.
    @param f the argument
    @return this
     */
    ScriptOverload& operator<<(float* f);

    /** Add a integer argument.
    @param i the argument
    @return this
     */
    ScriptOverload& operator<<(int* i);

    /** Add a string argument.
    @param s the argument
    @return this
     */
    ScriptOverload& operator<<(std::string* s);

  protected:
    /** Internal. Validate and update the supplied arguments.
    @param argcount the number of arguments.
    @param state the lua state.
    @returns true if everything validated and is updated, false if not.
     */
    bool validate(int argcount, lua_State* state);

  private:
    ScriptParams* params;
    std::vector<std::shared_ptr<internal::ScriptArgument>> arguments;
    bool valid;
};


/** Creates a light user data argument from a defined pointer.
@param t the pointer.
@return the light user data argument.
 */
template <typename T>
std::shared_ptr<internal::ScriptArgument> cLightUserData(T** t) {
  std::shared_ptr<internal::ScriptArgument> r(
    new internal::LightUserDataScriptArgument(reinterpret_cast<void**>(t)));
  return r;
}

/** Creates a full user data argument from a defined pointer.
@param t the pointer
@param name the name of the meta table
@return the full user data argument
 */
template <typename T>
std::shared_ptr<internal::ScriptArgument> cFullUserData(T** t,
    const std::string& name) {
  std::shared_ptr<internal::ScriptArgument> r(
    new internal::FullUserDataScriptArgument(reinterpret_cast<void**>(t),
        name));
  return r;
}

/** Creates a full user data argument from a defined pointer.
@param T the type
@param t the pointer
@return the full user data argument
 */
#define mFullUserData(T, t) cFullUserData<T>(t, #T)

/** Encapsulated utility class for getting arguments and returning values.
 */
class ScriptParams {
  public:
    /** Not explicitly called by the user. The user should use the
    REGISTERS_SCRIPT_FUNCTION macro.
    @param astate the state
     */
    explicit ScriptParams(lua_State* astate);

    /** Determine the overload to use or abort if no one can be found.
     */
    void post();

    /** Return some light userdata.
    @param userdata the light userdata to return
     */
    void returnvar(void* userdata);

    /** Return a float.
    @param f the float to return
     */
    void returnvar(float f);

    /** Return a string.
    @param s the string to return
     */
    void returnvar(const std::string& s);

    /** Helper function for returning full user data.
    Increases the return count by one and returns the state.
    @returns the lua state
     */
    lua_State* returnFullUserData();

    /** Internal. Gets the number of return values that has been added.
    @returns the return count.
     */
    int getReturnCount();

  protected:
    friend class ScriptOverload;

    /** Internal. Gets the number of arguments supplied to the lua function call.
    @returns the number of arguments.
     */
    int getArgumentCount();

    /** Internal. Gets the validated status.
    @return true if the item has been validated, false if not.
     */
    bool isValidated();

    /** Get the lua state.
    @returns the lua state
     */
    lua_State* getState();

    /** Marks the params as validated.
     */
    void setValidated();

    /** Add a validation failure for easier script debugging and better error
    messages.
    @param f the validation failure message/status.
     */
    void addFailure(const std::string& f);

  private:
    lua_State* state;
    int retcount;
    int argumentcount;
    bool validated;
    std::vector<std::string> failures;
};

/** Utility class for registring script functions.
 */
class ScriptRegister {
  public:
    /** Constructor.
     */
    ScriptRegister();

    /** Add a function.
    @param namespaceName the name of the lua namespace/table.
    @param name the name of the function.
    @param func the function.
     */
    void add(const std::string& namespaceName, const std::string& name,
             lua_CFunction func);

    /** Register all functions on a state.
    @param state the state to register to.
     */
    void registerAll(lua_State* state);
  private:
    typedef std::map<std::string, lua_CFunction> Functions;
    std::map<std::string, Functions> functions;
};

/** Get the global script registrer.
@returns a pointer to the global script register
 */
ScriptRegister* GetGlobalScriptRegister();

/** Basic script function
@param name the name of the function surrounded by quotes like "MyFunc"
@param func the callback function
@param luafunc the lua callback function name
 */
#define SCRIPT_FUNCTION(name, func, luafunc) \
  int luafunc(lua_State* state) { \
    try {\
      ScriptParams params(state);\
      func(&params);\
      params.post();\
      return params.getReturnCount();\
    } catch(...) {\
      return internal::HandleLuaException(name, state);\
    }\
  }

/** Register a script function. The function to add should have the following
structure:
void MyFunc(ScriptParams* params){ ]
@param name the name of the function surrounded by quotes like "MyFunc"
@param func the function to register
 */
#define REGISTER_SCRIPT_FUNCTION(name, func) \
  SCRIPT_FUNCTION(name, func, Lua_callback_for_##func)\
  class Lua_class_register_ ## func { \
    public:\
      Lua_class_register_##func() { \
        assert(GetGlobalScriptRegister());\
        GetGlobalScriptRegister()->add(LUA_MODULE_NAME, name, \
                                       Lua_callback_for_##func);\
      }\
  } Lua_static_var_register_ ## func

#endif  // EUPHORIA_SCRIPT_H_
