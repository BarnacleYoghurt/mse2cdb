#ifndef MSE2CDB_CDBCARD_HPP
#define MSE2CDB_CDBCARD_HPP

#include <string>
#include <vector>

namespace domain {
    struct CDBCard {

        //datas table
        unsigned int id;
        int ot;
        unsigned int alias;
        unsigned long setcode;
        unsigned int type;
        int atk;
        int def;
        unsigned long level;
        unsigned int race;
        unsigned int attribute;
        long category;

        //texts table
        std::string name;
        std::string desc;
        std::vector<std::string> str;

    };
}

#endif //MSE2CDB_CDBCARD_HPP
