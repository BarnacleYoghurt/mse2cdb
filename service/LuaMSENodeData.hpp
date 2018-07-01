#ifndef MSE2CDB_LUAMSENODEDATA_HPP
#define MSE2CDB_LUAMSENODEDATA_HPP

#include <domain/MSEDataNode.hpp>

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"

namespace service {
    namespace mseNodeData {
        int getChildValue(lua_State *state);
        int getChildFullContent(lua_State *state);
    }
}

#endif //MSE2CDB_LUAMSENODEDATA_HPP
