#ifndef MSE2CDB_LUARUNNER_HPP
#define MSE2CDB_LUARUNNER_HPP


#include <string>
#include <domain/MSEDataNode.hpp>

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"


namespace service {
    class LuaCardData {
    public:
        /**
         * @brief Initialize Lua state and load the specified script.
         * @param templatePath The Lua script to load.
         * @throw runtime_error if the state cannot be created or the script cannot be loaded.
         */
        LuaCardData(const std::string &templatePath);
        /**
         * @brief Automatically close the Lua state when the object is destroyed.
         */
        ~LuaCardData();

        /**
         * @brief Get id field using the corresponding function of the loaded script.
         * @param data The MSEDataNode from which id should be read.
         * @return The id returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int id(const domain::MSEDataNode &data);

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
}

#endif //MSE2CDB_LUARUNNER_HPP
