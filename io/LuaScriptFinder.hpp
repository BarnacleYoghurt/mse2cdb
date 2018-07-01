#ifndef MSE2CDB_LUASCRIPTFINDER_HPP
#define MSE2CDB_LUASCRIPTFINDER_HPP

#include <string>

namespace io {
    namespace luaScriptFinder {
        std::string getConstantsPath();

        std::string getUtilityPath();

        std::string getTemplatePath(const std::string &templateName);

        std::string getDictPath(const std::string &language);

        std::string getArchetypesPath(const std::string &language);
    }
}
#endif //MSE2CDB_LUASCRIPTFINDER_HPP
