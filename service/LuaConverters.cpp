#include "LuaConverters.hpp"

std::string lua_conv_toStdString(lua_State *L, int i) {
    const char *value = lua_tostring(L,i);
    return value == nullptr?"":value;
}

lua_Integer lua_conv_tointeger(lua_State *L, int i){
    return lua_tointeger(L, i);
}

std::vector<std::string> lua_conv_tostrlist(lua_State *L, int i){
    std::vector<std::string> strlist;

    lua_pushnil(L);
    while (lua_next(L,i<0?i-1:i) != 0) {
        auto value = lua_tostring(L, -1);
        strlist.emplace_back(value == nullptr ? "" : value);
        lua_pop(L,1);
    }
    return strlist;
}