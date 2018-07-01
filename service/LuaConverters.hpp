#ifndef MSE2CDB_LUACONVERTERS_HPP
#define MSE2CDB_LUACONVERTERS_HPP

#include <string>

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"

std::string lua_toStdString(lua_State *L, int i);

#endif //MSE2CDB_LUACONVERTERS_HPP
