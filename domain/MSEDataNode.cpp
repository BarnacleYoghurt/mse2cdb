#include "MSEDataNode.hpp"
#include <boost/algorithm/string.hpp>

namespace domain {
    MSEDataNode::MSEDataNode() {
    }

    void MSEDataNode::setValue(std::string value) {
        this->value = value;
    }

    void MSEDataNode::setOriginalLine(std::string originalLine) {
        this->originalLine = originalLine;
    }

    void MSEDataNode::addChild(std::string key, std::shared_ptr<MSEDataNode> node) {
        this->children.insert(std::make_pair(key, node));
    }

    std::string MSEDataNode::getValue() const {
        return value;
    }

    std::string MSEDataNode::getOriginalLine() const {
        return originalLine;
    }

    std::vector<MSEDataNode> MSEDataNode::getChildrenWithKey(std::string key) const {
        std::vector<MSEDataNode> childrenWithKey;

        auto bounds = children.equal_range(key);
        for (auto it = bounds.first; it!=bounds.second; it++){
            if (it->second != nullptr) {
                childrenWithKey.emplace_back(*(it->second));
            }
        }
        return childrenWithKey;
    }

    std::vector<std::string> MSEDataNode::getLines() const {
        std::vector<std::string> lines;
        if (!this->getValue().empty()){
            lines.emplace_back(this->getValue());
        }
        for (auto &child : children){
            std::string childLine = child.second->getOriginalLine();
            childLine.erase(childLine.begin(), find_if(childLine.begin(), childLine.end(), [](int c){ return !isspace(c); }));
            lines.emplace_back(childLine);
            std::vector<std::string> cLines = child.second->getLines();
            if (!child.second->getValue().empty()){
                cLines.erase(cLines.begin());
            }
            for (auto &cLine : cLines){
                //TODO: Revert to exactly the original indents (assuming a situation where it's necessary is possible)
                lines.emplace_back("\t" + cLine);
            }
        }

        return lines;
    }

    std::string MSEDataNode::toString() const{
        std::string result;

        std::vector<std::string> lines = getLines();
        for (auto &line : lines){
            result += line + "\n";
        }

        return result;
    }
}
