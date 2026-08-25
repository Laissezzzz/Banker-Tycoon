#pragma once

#include <sqlite3.h>
#include <iostream>
#include <string>

class Database {
private:
	sqlite3* database = nullptr;
public:
    Database() = default;

    void close() {
        if (database != nullptr) {
            sqlite3_close(database);
            database = nullptr;
        }
    }
    ~Database() {close();}

    bool open(const std::string& path) {
        close();

        const int result = sqlite3_open(path.c_str(), &database);

        if (result != SQLITE_OK) {
            std::cerr << "Errore apertura database: ";

            if (database != nullptr) {
                std::cerr << sqlite3_errmsg(database);
            }

            std::cerr << '\n';

            close();
            return false;
        }
        return true;
    }

    bool execute(const std::string& query) {
        if (database == nullptr) {
            std::cerr << "Database non aperto\n";
            return false;
        }

        char* errorMessage = nullptr;

        const int result = sqlite3_exec(database,query.c_str(),nullptr,nullptr,&errorMessage);

        if (result != SQLITE_OK) {
            std::cerr << "Errore query SQLite: ";

            if (errorMessage != nullptr) {
                std::cerr << errorMessage;
                sqlite3_free(errorMessage);
            }

            std::cerr << '\n';
            return false;
        }

        return true;
    }

    bool isOpen() const {
        return database != nullptr;
    }

    sqlite3* getHandle() const {
        return database;
    }
};