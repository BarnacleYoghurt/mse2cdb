#ifndef MSE2CDB_CDBACCESS_HPP
#define MSE2CDB_CDBACCESS_HPP

#include <string>
#include <sqlite3.h>
#include <domain/CDBCard.hpp>

namespace io {

    class CDBAccess {
    public:
        /**
         * @brief Initialze a CDBAccess object that allows access to the database at the given path.
         * @param dbPath The .cdb file to access.
         * @throw runtime_error if the database connection is not successfully opened.
         */
        CDBAccess(std::string dbPath);

        /**
         * @brief Automatically closes the database connection when the object is destroyed.
         */
        ~CDBAccess();

        /**
         * @brief Adds or updates the data of a single card.
         * @param card The card data to use for the operation.
         * @throw runtime_error If the SQLite statement cannot be created or executed properly.
         */
        void save(domain::CDBCard &card);

    private:
        sqlite3 *cdb = nullptr;
    };

}

#endif //MSE2CDB_CDBACCESS_HPP
