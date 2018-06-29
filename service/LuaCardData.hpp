#ifndef MSE2CDB_LUARUNNER_HPP
#define MSE2CDB_LUARUNNER_HPP


#include <string>
#include <lua5.1/lua.h>

class LuaCardData {
public:
    LuaCardData(std::string &templatePath);
    ~LuaCardData();
    unsigned int id();
    int ot();
    unsigned int alias();
    unsigned long setcode();
    unsigned int type();
    int atk();
    int def();
    unsigned long level();
    unsigned int race();
    unsigned int attribute();
    long category();

private:
    lua_State *state;
};


#endif //MSE2CDB_LUARUNNER_HPP
