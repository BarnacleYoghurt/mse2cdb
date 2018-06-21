#include "MSEDataNode.hpp"

namespace domain {
    MSEDataNode::MSEDataNode(std::shared_ptr<MSEDataNode> parent) : parent(parent) {
    }

    void MSEDataNode::setValue(std::string value) {
        this->value = value;
    }

    void MSEDataNode::addChild(std::string key, domain::MSEDataNode node) {
        this->children.insert(std::make_pair(key, node));
    }

    std::string MSEDataNode::getValue() {
        return value;
    }

    std::shared_ptr<MSEDataNode> MSEDataNode::getParent(){
        return parent;
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
