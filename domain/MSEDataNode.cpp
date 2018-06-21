#include "MSEDataNode.hpp"

namespace domain {
    MSEDataNode::MSEDataNode(std::shared_ptr<MSEDataNode> parent) : parent(parent) {

    }

    std::string MSEDataNode::getValue() {
        return value;
    }

    MSEDataNode MSEDataNode::getChildNode(std::string key) {
        std::multimap::iterator it = children.find(key);
        if (it != children.end()) {
            return it->second;
        } else {
            return MSEDataNode(*this);
        }
    }

    std::string MSEDataNode::getChildValue(std::string key) {
        return getChildNode(key).getValue();
    }
}
