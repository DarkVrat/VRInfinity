#pragma once

#include <iostream>

namespace DB
{
    class DBController {
    public:
        DBController(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
        DBController(const DBController& dbController)                      = delete;
        DBController(DBController&& dbController)               noexcept    = default;
        DBController& operator=(const DBController& dbController)           = delete;
        DBController& operator=(DBController&& dbController)    noexcept    = default;
        ~DBController();

        void connect();
        void disconnect();

        void startTransaction();
        void commitTransaction();
        void rollbackTransaction();



    private:
        std::string m_host;
        std::string m_user;
        std::string m_password;
        std::string m_database;
    };
}
