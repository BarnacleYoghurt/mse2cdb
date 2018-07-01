#ifndef MSE2CDB_LUACONVERTERS_HPP
#define MSE2CDB_LUACONVERTERS_HPP

#include <string>

#include "lua5.3/lua.h"
#include "lua5.3/lauxlib.h"
#include "lua5.3/lualib.h"

std::string lua_conv_toStdString(lua_State *L, int i);
lua_Integer lua_conv_tointeger(lua_State *L, int i);

#endif //MSE2CDB_LUACONVERTERS_HPP
