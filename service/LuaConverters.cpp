#include "LuaConverters.hpp"

std::string lua_conv_toStdString(lua_State *L, int i) {
    const char *value = lua_tostring(L,i);
    return value == nullptr?"":value;
}

lua_Integer lua_conv_tointeger(lua_State *L, int i){
    return lua_tointeger(L, i);
}