#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>
#include <memory>
namespace domain {
    class MSEDataNode {
    public:
        MSEDataNode();
        void setValue(std::string value);
        void addChild(std::string key, std::shared_ptr<MSEDataNode> node);
        std::string getValue();
        std::string getChildValue(std::string key);
        MSEDataNode getChildNode(std::string key);
        std::string toString();
    private:
        std::multimap<std::string, std::shared_ptr<MSEDataNode>> children;
        std::string value;
    };
}

#endif //MSE2CDB_MSEDATA_HPP
