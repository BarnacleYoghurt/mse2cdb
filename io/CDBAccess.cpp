#include <stdexcept>
#include "CDBAccess.hpp"
namespace io {

    CDBAccess::CDBAccess(std::string dbPath) {
        if (sqlite3_open(dbPath.c_str(), &cdb) != SQLITE_OK) {
            throw std::runtime_error(
                    "Could not open connection to cdb due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
        }
    }

    CDBAccess::~CDBAccess() {
        if (cdb != nullptr) {
            sqlite3_close(cdb);
        }
    }

    int CDBAccess::getCardCount() {
        sqlite3_stmt *getCountStmt;
        if (sqlite3_prepare_v2(cdb, "SELECT COUNT(*) FROM datas", -1, &getCountStmt, 0) != SQLITE_OK) {
            throw std::runtime_error(
                    "Could not get card count due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
        }
        if (sqlite3_step(getCountStmt) != SQLITE_ROW) {
            throw std::runtime_error(
                    "Could not get card count due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
        }
        return sqlite3_column_int(getCountStmt, 0);
    }
}