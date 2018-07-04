#ifndef MSE2CDB_LUAMSENODEDATA_HPP
#define MSE2CDB_LUAMSENODEDATA_HPP

#include <domain/MSEDataNode.hpp>

#include "lua5.3/lua.h"
#include "lua5.3/lauxlib.h"
#include "lua5.3/lualib.h"

namespace service {
    namespace mseNodeData {
        /**
         * @brief Used by Lua to get the first child node with a certain key from an MSEDataNode.
         * @details Called from lua with <node>:GetChildNode(<key>), where node is an MSEDataNode object and key is a string.
         * Lua return value is an MSEDataNode as well (an empty one if the key wasn't found), but is only stored until the next time this function is called!
         * @param state The lua state calling the function.
         * @return The number of values returned to Lua (1).
         */
        int getChildNode(lua_State *state);
        /**
         * @brief Used by Lua to get the value of the first child node with a certain key from an MSEDataNode.
         * @details Called from lua with <node>:GetChildValue(<key>), where node is an MSEDataNode object and key is a string.
         * Lua return value is a string (an empty one if the key wasn't found).
         * @param state The lua state calling the function.
         * @return The number of values returned to Lua (1).
         */
        int getChildValue(lua_State *state);
        /**
         * @brief Used by Lua to get the full original string content of the first child note with a certain key from an MSEDataNode.
         * @details Called from lua with <node>:GetChildFullContent(<key>), where node is an MSEDataNode object and key is a string.
         * Lua return value is a string (an empty one if the key wasn't found).
         * @param state The lua state calling the function.
         * @return The number of values returned to Lua (1).
         */
        int getChildFullContent(lua_State *state);
    }
}

#endif //MSE2CDB_LUAMSENODEDATA_HPP
