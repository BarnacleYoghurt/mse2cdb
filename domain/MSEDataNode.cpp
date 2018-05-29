#include "MSEDataNode.hpp"

MSEDataNode::MSEDataNode() {

}

MSEDataNode::MSEDataNode(std::string mseData){
    //TODO: Transform textual data from MSE into tree
}

std::string MSEDataNode::getValue() {
    return value;
}

MSEDataNode MSEDataNode::getChildNode(std::string key) {
    if (children.find(key) != children.end()){
        return children[key];
    }
    else{
        return MSEDataNode();
    }
}

std::string MSEDataNode::getChildValue(std::string key) {
    return getChildNode(key).getValue();
}

