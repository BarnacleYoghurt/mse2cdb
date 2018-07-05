#include "MSEDataNode.hpp"
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace domain {
    MSEDataNode::MSEDataNode(std::string sourceText) : sourceText(sourceText) {
        std::stringstream ss(sourceText);
        std::string line;

        int baseIndent = -1;
        std::string currKey;
        std::string currValue;
        std::string currSourceText;

        while (std::getline(ss,line)){
            if (baseIndent < 0){
                baseIndent = 0;
                for (char &c : line){
                    if (c != '\t'){
                        break;
                    }
                    baseIndent++;
                }
            }
            int currIndent = 0;
            for (char &c : line){
                if (c != '\t'){
                    break;
                }
                currIndent++;
            }

            if (currIndent < baseIndent){
                baseIndent = currIndent;
            }

            if (currIndent == baseIndent){
                if (!(currKey.empty() && currValue.empty() && currSourceText.empty())){
                    //Create and insert pending child
                    MSEDataNode child(currSourceText);
                    child.setValue(currValue);
                    this->addChild(currKey, child);

                    //Clear values to prepare for next child
                    currKey.clear();
                    currValue.clear();
                    currSourceText.clear();
                }
                //Read values for next child
                size_t sepPos = line.find(':');
                if (sepPos != std::string::npos){
                    currKey = line.substr(0,sepPos);
                    currValue = line.substr(sepPos + 1);
                }
                else{
                    currValue = line;
                }
                currKey.erase(currKey.begin(), find_if(currKey.begin(), currKey.end(), [](int c){ return !isspace(c); }));
                currValue.erase(currValue.begin(), find_if(currValue.begin(), currValue.end(), [](int c){ return !isspace(c); }));
            }
            else{
                if (!currSourceText.empty()){
                    currSourceText += "\n";
                }
                currSourceText += line.erase(0, (unsigned long)baseIndent+1);
            }
        }

        if (!(currKey.empty() && currValue.empty() && currSourceText.empty())){
            //Create and insert pending child
            MSEDataNode child(currSourceText);
            child.setValue(currValue);
            this->addChild(currKey, child);
        }
    }

    void MSEDataNode::setValue(std::string value) {
        this->value = value;
    }

    void MSEDataNode::addChild(std::string key, MSEDataNode node) {
        this->children.insert(std::make_pair(key, node));
    }

    std::string MSEDataNode::getValue() const {
        return value;
    }

    std::string MSEDataNode::getSourceText() const {
        return sourceText;
    }

    std::vector<MSEDataNode> MSEDataNode::getChildrenWithKey(std::string key) const {
        std::vector<MSEDataNode> childrenWithKey;

        auto bounds = children.equal_range(key);
        for (auto it = bounds.first; it!=bounds.second; it++){
            childrenWithKey.emplace_back(it->second);
        }
        return childrenWithKey;
    }
}
