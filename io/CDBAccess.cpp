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

    void CDBAccess::save(domain::CDBCard &card) {
        const char *datasInsertSql = "INSERT OR IGNORE INTO datas (id) VALUES(?);";
        const char *textsInsertSql = "INSERT OR IGNORE INTO texts (id) VALUES(?);";
        const char *datasUpdateSql = "UPDATE datas set ot=?, alias=?, setcode=?, type=?, atk=?, def=?, level=?, race=?, attribute=?, category=? WHERE id=?;";
        const char *textsUpdateSql = "UPDATE texts set name=?, desc=?, "
                                        "str1=?, str2=?, str3=?, str4=?, str5=?, str6=?, str7=?, str8=?, "
                                        "str9=?, str10=?, str11=?, str12=?, str13=?, str14=?, str15=?, str16=? "
                                        "WHERE id=?;";

        char *errMsg;
        if (sqlite3_exec(cdb, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throw std::runtime_error(
                    "Could not save card due to SQLite error (" + std::string(errMsg) + ")");
        }

        sqlite3_stmt *datasInsertStmt;
        sqlite3_stmt *textsInsertStmt;
        sqlite3_stmt *datasUpdateStmt;
        sqlite3_stmt *textsUpdateStmt;

        try {
            if (sqlite3_prepare_v2(cdb, datasInsertSql, -1, &datasInsertStmt, 0) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_prepare_v2(cdb, textsInsertSql, -1, &textsInsertStmt, 0) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_prepare_v2(cdb, datasUpdateSql, -1, &datasUpdateStmt, 0) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_prepare_v2(cdb, textsUpdateSql, -1, &textsUpdateStmt, 0) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }

            //Bind parameters
            if (sqlite3_bind_int(datasInsertStmt, 1, card.id) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(textsInsertStmt, 1, card.id) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 1, card.ot) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 2, card.alias) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int64(datasUpdateStmt, 3, (sqlite3_uint64) card.setcode) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 4, card.type) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 5, card.atk) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 6, card.def) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int64(datasUpdateStmt, 7, (sqlite_uint64) card.level) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 8, card.race) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 9, card.attribute) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int64(datasUpdateStmt, 10, card.category) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_int(datasUpdateStmt, 11, card.id) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_text(textsUpdateStmt, 1, card.name.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_bind_text(textsUpdateStmt, 2, card.desc.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            for (int i = 0; i < 16; i++) {
                if (sqlite3_bind_text(textsUpdateStmt, 3 + i, card.str[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
                    throw std::runtime_error(
                            "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
                }
            }
            if (sqlite3_bind_int(textsUpdateStmt, 19, card.id) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }

            if (sqlite3_step(datasInsertStmt) != SQLITE_DONE) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_step(textsInsertStmt) != SQLITE_DONE) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_step(datasUpdateStmt) != SQLITE_DONE) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_step(textsUpdateStmt) != SQLITE_DONE) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(sqlite3_errmsg(cdb)) + ")");
            }
            if (sqlite3_exec(cdb, "COMMIT TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
                throw std::runtime_error(
                        "Could not save card due to SQLite error (" + std::string(errMsg) + ")");
            }
        }
        catch (const std::exception &e){
            sqlite3_exec(cdb, "ROLLBACK TRANSACTION;", nullptr, nullptr, nullptr);
            throw;
        }
    }
}