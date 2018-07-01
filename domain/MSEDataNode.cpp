#include "MSEDataNode.hpp"
#include <boost/algorithm/string.hpp>

namespace domain {
    MSEDataNode::MSEDataNode() {
    }

    void MSEDataNode::setValue(std::string value) {
        this->value = value;
    }

    void MSEDataNode::addChild(std::string key, std::shared_ptr<MSEDataNode> node) {
        this->children.insert(std::make_pair(key, node));
    }

    std::string MSEDataNode::getValue() const {
        return value;
    }

    MSEDataNode MSEDataNode::getChildNode(std::string key) const {
        auto it = children.find(key);
        if (it != children.end() && it->second != nullptr) {
            return *(it->second);
        } else {
            return MSEDataNode();
        }
    }

    std::string MSEDataNode::getChildValue(std::string key) const {
        return getChildNode(key).getValue();
    }

    std::string MSEDataNode::toString() const{
        std::string result;
        if (!value.empty()){
            result += value;
        }
        for (auto &child : children){
            std::string part = "\n";
            part += child.first + ": ";
            part += child.second->toString();
            boost::replace_all(part, "\n", "\n\t");
            result += part;
        }

        return result;
    }
}
