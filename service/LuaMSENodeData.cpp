#include "LuaMSENodeData.hpp"

#include <cstring>

namespace service{
    namespace mseNodeData {

        int getChildNode(lua_State *state) {
            //Return value must not go out of scope when the function ends
            static domain::MSEDataNode childNode;

            auto node = (domain::MSEDataNode*)lua_touserdata(state, 1);
            const char *key = lua_tostring(state, 2);
            if (key != nullptr) {
                childNode = node->getChildNode(key);
            }

            lua_pushlightuserdata(state, &childNode);

            return 1;
        }

        int getChildValue(lua_State *state) {
            auto node = (domain::MSEDataNode*)lua_touserdata(state, 1);
            const char *key = lua_tostring(state, 2);
            if (key != nullptr) {
                lua_pushstring(state, node->getChildValue(key).c_str());
            } else {
                lua_pushstring(state, "");
            }
            return 1;
        }

        int getChildFullContent(lua_State *state){
            auto node = (domain::MSEDataNode*)lua_touserdata(state, 1);
            const char *key = lua_tostring(state, 2);
            if (key != nullptr) {
                lua_pushstring(state, node->getChildNode(key).toString().c_str());
            } else {
                lua_pushstring(state, "");
            }
            return 1;
        }
    }
}