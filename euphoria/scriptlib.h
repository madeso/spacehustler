  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Functions for binding basic types to lua.
 */

#ifndef EUPHORIA_SCRIPTLIB_H_
#define EUPHORIA_SCRIPTLIB_H_

#include "euphoria/math.h"

extern "C" {
  struct lua_State;
}

/** Add the scriptlib to the state.
@param state the lua state.
 */
void scriptlib_register(lua_State* state);

class ScriptParams;

/** Add a return quaternion.
@param params the lua params
@returns the returned quaternion
 */
quat* ReturnQuat(ScriptParams* params);

/** Add a return vec3.
@param params the lua params
@returns the returned vec3
 */
vec3* ReturnVec3(ScriptParams* params);

#endif  // EUPHORIA_SCRIPTLIB_H_
