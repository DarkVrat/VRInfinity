#include "DBController.h"

using namespace DB;

DBController::DBController(const std::string& settings) : m_settings(settings)
{
    try
    {
        m_conn.connect(m_settings);
    }
    catch (const nanodbc::database_error& e) 
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

DBController::DBController(const DBController& dbController) : m_settings(dbController.m_settings)
{
    try
    {
        m_conn.connect(m_settings);
    }
    catch (const nanodbc::database_error& e) 
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

DBController::DBController(DBController&& dbController) noexcept
    : m_settings(dbController.m_settings), m_conn(std::move(dbController.m_conn)) {}

DBController& DBController::operator=(const DBController& dbController)
{
    if (this == &dbController)
        return *this;

    m_conn.disconnect();
    m_settings = dbController.m_settings;
    m_conn.connect(m_settings);

    return *this;
}

DBController& DBController::operator=(DBController&& dbController) noexcept
{
    if (this == &dbController)
        return *this;

    m_conn.disconnect();

    m_settings = dbController.m_settings;
    m_conn = std::move(dbController.m_conn);

    dbController.m_settings = "";

    return *this;
}

DBController::~DBController()
{
    try
    {
        m_conn.disconnect();
    }
    catch (const nanodbc::database_error& e) 
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

void DBController::startTransaction()
{
    try
    {
        nanodbc::execute(m_conn, "BEGIN TRANSACTION");
    }
    catch (const nanodbc::database_error& e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

void DBController::commitTransaction()
{
    try
    {
        nanodbc::execute(m_conn, "COMMIT");
    }
    catch (const nanodbc::database_error& e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

void DBController::rollbackTransaction()
{
    try
    {
        nanodbc::execute(m_conn, "ROLLBACK");
    }
    catch (const nanodbc::database_error& e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

bool DBController::execute(const std::string& query)
{
    try
    {
        nanodbc::execute(m_conn, query);
    }
    catch (const nanodbc::database_error& e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

nanodbc::statement DBController::statement(const std::string& query)
{
    return nanodbc::statement(m_conn, query);
}
