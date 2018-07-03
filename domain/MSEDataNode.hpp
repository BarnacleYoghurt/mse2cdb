#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>
#include <memory>
#include <vector>

namespace domain {
    class MSEDataNode {
    public:
        MSEDataNode();
        void setValue(std::string value);
        void setOriginalLine(std::string originalLine);
        void addChild(std::string key, std::shared_ptr<MSEDataNode> node);
        std::string getValue() const;
        std::string getOriginalLine() const;
        std::string getChildValue(std::string key) const;
        MSEDataNode getChildNode(std::string key) const;
        std::vector<std::string> getLines() const;
        std::string toString() const;
    private:
        std::multimap<std::string, std::shared_ptr<MSEDataNode>> children;
        std::string value;
        std::string originalLine;
    };
}

#endif //MSE2CDB_MSEDATA_HPP
