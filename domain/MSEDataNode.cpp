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

    MSEDataNode MSEDataNode::getChildNode(std::string key) const {
        auto it = children.find(key);
        if (it != children.end() && it->second != nullptr) {
            return *(it->second);
        } else {
            return MSEDataNode();
        }
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

    std::string MSEDataNode::getChildValue(std::string key) const {
        return getChildNode(key).getValue();
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
