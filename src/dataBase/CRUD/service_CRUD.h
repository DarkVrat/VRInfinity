#pragma once

#include <vector>
#include "../tables/service.h"
#include "../DBController.h"

namespace DB
{
    class service_CRUD
    {
    public:
        static std::vector<service> getAllServices(DBController* dbController)
        {
            std::vector<service> Services;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, formula FROM service;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    service Service;
                    Service.setId(results.get<uint32_t>(0));
                    Service.setName(results.get<std::string>(1));
                    Service.setFormula(results.get<std::string>(2));

                    Services.push_back(std::move(Service));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return Services;
        }

        static service getServiceByID(DBController* dbController, uint32_t id)
        {
            service Service;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, formula FROM service WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    Service.setId(results.get<uint32_t>(0));
                    Service.setName(results.get<std::string>(1));
                    Service.setFormula(results.get<std::string>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return Service;
        }

        static uint32_t createService(DBController* dbController, const service& Service)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO service (id, name, formula) VALUES (?, ?, ?);"));
                stmt.bind(0, &Service.getId());
                stmt.bind(1, Service.getName().c_str());
                stmt.bind(2, Service.getFormula().c_str());
                nanodbc::execute(stmt);

                nanodbc::statement id_stmt(dbController->statement("SELECT LAST_INSERT_ID();"));
                auto result = nanodbc::execute(id_stmt);
                if (result.next())
                    return result.get<int>(0);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return 0;
            }
            return 0;
        }

        static bool updateService(DBController* dbController, const service& Service)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE service SET name = ?, formula = ? WHERE id = ?;"));
                stmt.bind(0, Service.getName().c_str());
                stmt.bind(1, Service.getFormula().c_str());
                stmt.bind(2, &Service.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteService(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM service WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }
    };
}
