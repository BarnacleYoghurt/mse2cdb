#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>
#include <memory>
#include <vector>

namespace domain {
    class MSEDataNode {
    public:
        MSEDataNode(std::string sourceText);
        void setValue(std::string value);
        void addChild(std::string key, MSEDataNode node);
        std::string getValue() const;
        std::string getSourceText() const;
        std::vector<MSEDataNode> getChildrenWithKey(std::string key) const;
    private:
        std::multimap<std::string, MSEDataNode> children;
        std::string value;
        std::string sourceText;
    };
}

#endif //MSE2CDB_MSEDATA_HPP
