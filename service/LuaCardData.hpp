#ifndef MSE2CDB_LUARUNNER_HPP
#define MSE2CDB_LUARUNNER_HPP


#include <string>
#include <domain/MSEDataNode.hpp>

#include "lua5.3/lua.h"
#include "lua5.3/lauxlib.h"
#include "lua5.3/lualib.h"
#include "LuaMSENodeData.hpp"


namespace service {
    class LuaCardData {
    public:
        /**
         * @brief Initialize Lua state and load the specified script.
         * @param templateName The Lua script to load.
         * @throw runtime_error if the state cannot be created or the script cannot be loaded.
         */
        LuaCardData(const std::string &templateName, const std::string &language);
        /**
         * @brief Automatically close the Lua state when the object is destroyed.
         */
        ~LuaCardData();

        /**
         * @brief Get id field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which id should be read.
         * @return The id returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int id(const domain::MSEDataNode &data);
        /**
         * @brief Get ot field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which ot should be read.
         * @return The id returned by the function in the script; 0 if that value is not a number.
         */
        int ot(const domain::MSEDataNode &data);
        /**
         * @brief Get alias field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which alias should be read.
         * @return The alias returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int alias(const domain::MSEDataNode &data);
        /**
         * @brief Get setcode field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which setcode should be read.
         * @return The setcode returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned long setcode(const domain::MSEDataNode &data);
        /**
         * @brief Get type field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which type should be read.
         * @return The type returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int type(const domain::MSEDataNode &data);
        /**
         * @brief Get atk field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which atk should be read.
         * @return The atk returned by the function in the script; 0 if that value is not a number.
         */
        int atk(const domain::MSEDataNode &data);
        /**
         * @brief Get def field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which def should be read.
         * @return The def returned by the function in the script; 0 if that value is not a number.
         */
        int def(const domain::MSEDataNode &data);
        /**
         * @brief Get level field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which level should be read.
         * @return The level returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned long level(const domain::MSEDataNode &data);
        /**
         * @brief Get race field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which race should be read.
         * @return The race returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int race(const domain::MSEDataNode &data);
        /**
         * @brief Get attribute field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which attribute should be read.
         * @return The attribute returned by the function in the script; 0 if that value is negative or not a number.
         */
        unsigned int attribute(const domain::MSEDataNode &data);
        /**
         * @brief Get category field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which category should be read.
         * @return The category returned by the function in the script; 0 if that value is not a number.
         */
        long category(const domain::MSEDataNode &data);

        /**
         * @brief Get name field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which name should be read.
         * @return The name returned by the function in the script.
         */
        std::string name(const domain::MSEDataNode &data);
        /**
         * @brief Get desc field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which desc should be read.
         * @return The desc returned by the function in the script.
         */
        std::string desc(const domain::MSEDataNode &data);
        /**
         * @brief Get str field using the corresponding function of the loaded template script.
         * @param data The MSEDataNode from which str should be read.
         * @return The str returned by the function in the script.
         */
        std::array<std::string, 16> str(const domain::MSEDataNode &data);

        template<typename T>
        int pcallCardDataFunction(const char *functionName, const domain::MSEDataNode &data, T *outVar, T (*converter)(lua_State*, int)){
            lua_CFunction protectedFunction = [](lua_State *state){
                const char *functionNameParam = lua_tostring(state, 1);
                auto *converterParam = reinterpret_cast<T (*)(lua_State*, int)>(lua_touserdata(state, 2));
                auto *resultParam = static_cast<T*>(lua_touserdata(state, 3));
                auto *dataParam = static_cast<domain::MSEDataNode*>(lua_touserdata(state, 4));

                lua_getglobal(state, "cd");
                lua_getfield(state, -1, functionNameParam);
                lua_pushlightuserdata(state, dataParam);

                luaL_getmetatable(state, "NodeData"); //Put metatable on stack
                lua_setmetatable(state, -2); //and pop it off again

                lua_call(state,1,1);
                *resultParam = converterParam(state, -1);
                return 0;
            };

            lua_pushcfunction(state, protectedFunction);
            lua_pushstring(state, functionName);
            lua_pushlightuserdata(state, reinterpret_cast<void*>(converter));
            lua_pushlightuserdata(state, outVar);
            lua_pushlightuserdata(state, (domain::MSEDataNode*)&data);
            return lua_pcall(state,4,0,0);
        }

    private:
        lua_State *state;
        void createNodeDataMetatable();
    };
}

#endif //MSE2CDB_LUARUNNER_HPP
