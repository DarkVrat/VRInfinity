#pragma once

#include <iostream>
#include <nanodbc/nanodbc.h>

namespace DB
{
    class DBController {
    public:
        DBController(const std::string& settings);
        DBController(const DBController& dbController);
        DBController(DBController&& dbController)               noexcept;
        DBController& operator=(const DBController& dbController);
        DBController& operator=(DBController&& dbController)    noexcept;
        ~DBController();

        void startTransaction();
        void commitTransaction();
        void rollbackTransaction();

        bool execute(const std::string& query);
        nanodbc::statement statement(const std::string& query);
    private:
        std::string m_settings;
        nanodbc::connection m_conn;
    };
}
