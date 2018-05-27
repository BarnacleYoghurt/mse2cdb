#ifndef MSE2CDB_CDBACCESS_HPP
#define MSE2CDB_CDBACCESS_HPP

#include <string>
#include <sqlite3.h>
namespace io {

    class CDBAccess {
    public:
        /**
         * Initialze a CDBAccess object that allows access to the database at the given path.
         * @param dbPath The .cdb file to access.
         * @throw runtime_error if the database connection is not successfully opened.
         */
        CDBAccess(std::string dbPath);

        /**
         * Automatically closes the database connection when the object is destroyed.
         */
        ~CDBAccess();

        /**
         * Used for debug and test purposes.
         * @return The number of entries in the datas table.
         * @throw runtime_error If the SQLite statement cannot be created or executed properly.
         */
        int getCardCount();

    private:
        sqlite3 *cdb = nullptr;
    };

}

#endif //MSE2CDB_CDBACCESS_HPP
