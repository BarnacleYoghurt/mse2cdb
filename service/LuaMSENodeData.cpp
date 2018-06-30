#include "LuaMSENodeData.hpp"
namespace service{
    namespace mseNodeData {

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
    }
}