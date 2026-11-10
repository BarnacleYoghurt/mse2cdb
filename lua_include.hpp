#ifndef MSE2CDB_LUAINCLUDE_HPP
#define MSE2CDB_LUAINCLUDE_HPP

/* http://lua-users.org/wiki/BuildingLua says:
 * "If lua was compiled as a C library, which is typical with pre-packaged binaries, [...]
 *  you will have to place extern "C" around the inclusion of the Lua headers in your C++ application"
 *
 * Nested extern "C" is legal, so this should be fine for any version of lua.
*/
extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#endif //MSE2CDB_LUAINCLUDE_HPP