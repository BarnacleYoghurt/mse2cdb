#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>
#include <memory>
namespace domain {
    class MSEDataNode {
    public:
        MSEDataNode(std::shared_ptr<MSEDataNode> parent = nullptr);
        void setValue(std::string value);
        void addChild(std::string key, MSEDataNode node);
        std::string getValue();
        std::shared_ptr<MSEDataNode> getParent();
        std::string getChildValue(std::string key);
        MSEDataNode getChildNode(std::string key);
    private:
        std::shared_ptr<MSEDataNode> parent;
        std::multimap<std::string, MSEDataNode> children;
        std::string value;
    };
}

#endif //MSE2CDB_MSEDATA_HPP
