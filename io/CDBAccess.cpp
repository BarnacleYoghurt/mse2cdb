#include "CDBAccess.hpp"
using namespace io;

CDBAccess::CDBAccess(std::string dbPath) {
    if (sqlite3_open(dbPath.c_str(), &cdb) != SQLITE_OK){
        //TODO: Handle error
    }
}

CDBAccess::~CDBAccess() {
    if (cdb != nullptr) {
        sqlite3_close(cdb);
    }
}

int CDBAccess::getCardCount() {
    sqlite3_stmt *getCountStmt;
    if (sqlite3_prepare_v2(cdb, "SELECT COUNT(*) FROM datas", -1, &getCountStmt, 0) != SQLITE_OK){
        //TODO: Handle error
    }
    if (sqlite3_step(getCountStmt) != SQLITE_ROW){
        //TODO: Handle error
    }
    return sqlite3_column_int(getCountStmt, 0);
}