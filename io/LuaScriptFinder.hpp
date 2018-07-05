#ifndef MSE2CDB_LUASCRIPTFINDER_HPP
#define MSE2CDB_LUASCRIPTFINDER_HPP

#include <string>

namespace io {
    namespace luaScriptFinder {
        /**
         * @return Path to constants.lua, either absolute or relative to the executable's location.
         */
        std::string getConstantsPath();
        /**
         * @return Path to utility.lua, either absolute or relative to the executable's location.
         */
        std::string getUtilityPath();
        /**
         * @param templateName Name of the template to find.
         * @return Path to <templateName>.lua, either absolute or relative to the executable's location.
         */
        std::string getTemplatePath(const std::string &templateName);
        /**
         * @param language Name of the language used.
         * @return Path to dict.lua for the given language, either absolute or relative to the executable's location.
         */
        std::string getDictPath(const std::string &language);
        /**
         * @param language Name of the language used.
         * @return Path to archetypes.lua for the given language, either absolute or relative to the executable's location.
         */
        std::string getArchetypesPath(const std::string &language);
    }
}
#endif //MSE2CDB_LUASCRIPTFINDER_HPP
