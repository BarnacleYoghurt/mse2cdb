#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>

class MSEDataNode {
public:
    MSEDataNode();
    MSEDataNode(std::string mseData);
    std::string getValue();
    std::string getChildValue(std::string key);
    MSEDataNode getChildNode(std::string key);
private:
    std::map<std::string, MSEDataNode> children;
    std::string value;
};


#endif //MSE2CDB_MSEDATA_HPP
