#ifndef MSE2CDB_MSEDATA_HPP
#define MSE2CDB_MSEDATA_HPP

#include <map>
#include <memory>
#include <vector>

namespace domain {
    class MSEDataNode {
    public:
        /**
         * @brief Recursively generate node and all its children based on text from an MSE set file.
         * @param sourceText The text from the MSE set file.
         */
        MSEDataNode(std::string sourceText);
        /**
         * @param value The new value of the node.
         */
        void setValue(std::string value);
        /**
         * @brief Add a child node with the given key.
         * @param key The key of the new child node.
         * @param node The new child node.
         */
        void addChild(std::string key, MSEDataNode node);
        /**
         * @return The value of the node.
         */
        std::string getValue() const;
        /**
         * @return The text the node was generated from.
         */
        std::string getSourceText() const;
        /**
         * @brief Get all children with the given key.
         * @param key The key.
         * @return A list of all direct child nodes that have the given key, in the same order as they are stored internally.
         */
        std::vector<MSEDataNode> getChildrenWithKey(std::string key) const;
    private:
        std::multimap<std::string, MSEDataNode> children;
        std::string value;
        std::string sourceText;
    };
}

#endif //MSE2CDB_MSEDATA_HPP
