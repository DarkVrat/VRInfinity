#include "DBController.h"

using namespace DB;

DBController::DBController(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
    : m_host(host), m_user(user), m_password(password), m_database(database)
{
    
}

DBController::~DBController()
{
    disconnect();
}

void DBController::connect()
{
    
}

void DBController::disconnect()
{
    
}

void DBController::startTransaction()
{
    
}

void DBController::commitTransaction()
{
    
}

void DBController::rollbackTransaction()
{
    
}
