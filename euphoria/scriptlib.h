// Euphoria - Copyright (c) Gustav

/** @file
Functions for binding basic types to lua.
 */

#ifndef EUPHORIA_SCRIPTLIB_H_
#define EUPHORIA_SCRIPTLIB_H_

extern "C" {
  struct lua_State;
}

/** Add the scriptlib to the state.
@param state the lua state.
 */
void scriptlib_register(lua_State* state);

#endif  // EUPHORIA_SCRIPTLIB_H_
