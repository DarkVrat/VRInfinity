#pragma once

#include <vector>
#include "../tables/visit.h"
#include "../DBController.h"

namespace DB
{
    class visit_CRUD
    {
    public:
        visit_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<visit> getAllVisitsByPhone(DBController* dbController, const std::string& phone)
        {
            std::vector<visit> visits;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, phone, start, service_id FROM visit WHERE phone = ?;"));
                stmt.bind(0, phone.c_str());
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visit Visit;
                    Visit.setId(results.get<uint32_t>(0));
                    Visit.setName(results.get<std::string>(1));
                    Visit.setSurname(results.get<std::string>(2));
                    Visit.setPhone(results.get<std::string>(3));
                    Visit.setStart(results.get<std::string>(4));
                    Visit.setServiceId(results.get<uint32_t>(5));

                    visits.push_back(std::move(Visit));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visits;
        }

        static std::vector<visit> getAllVisits(DBController* dbController)
        {
            std::vector<visit> visits;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, phone, start, service_id FROM visit;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visit Visit;
                    Visit.setId(results.get<uint32_t>(0));
                    Visit.setName(results.get<std::string>(1));
                    Visit.setSurname(results.get<std::string>(2));
                    Visit.setPhone(results.get<std::string>(3));
                    Visit.setStart(results.get<std::string>(4));
                    Visit.setServiceId(results.get<uint32_t>(5));

                    visits.push_back(std::move(Visit));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visits;
        }

        static visit getVisitByID(DBController* dbController, uint32_t id)
        {
            visit Visit;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, phone, start, service_id FROM visit WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    Visit.setId(results.get<uint32_t>(0));
                    Visit.setName(results.get<std::string>(1));
                    Visit.setSurname(results.get<std::string>(2));
                    Visit.setPhone(results.get<std::string>(3));
                    Visit.setStart(results.get<std::string>(4));
                    Visit.setServiceId(results.get<uint32_t>(5));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return Visit;
        }

        static bool createVisit(DBController* dbController, const visit& Visit)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO visit (name, surname, phone, start, service_id) VALUES (?, ?, ?, ?, ?);"));
                stmt.bind(0, Visit.getName().c_str());
                stmt.bind(1, Visit.getSurname().c_str());
                stmt.bind(2, Visit.getPhone().c_str());
                stmt.bind(3, Visit.getStart().c_str());
                stmt.bind(4, &Visit.getServiceId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateVisit(DBController* dbController, const visit& Visit)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE visit SET name = ?, surname = ?, phone ?, start = ?, service_id = ? WHERE id = ?;"));
                stmt.bind(0, Visit.getName().c_str());
                stmt.bind(1, Visit.getSurname().c_str());
                stmt.bind(2, Visit.getPhone().c_str());
                stmt.bind(3, Visit.getStart().c_str());
                stmt.bind(4, &Visit.getServiceId());
                stmt.bind(5, &Visit.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteVisit(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM visit WHERE id = ?;"));
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

        inline std::vector<visit> getAllVisitsByPhone(const std::string& phone) { return getAllVisitsByPhone(m_dbController, phone); }
        inline std::vector<visit> getAllVisits()        { return getAllVisits(m_dbController);      }
        inline visit    getVisitByID(uint32_t id)       { return getVisitByID(m_dbController, id);  }
        inline bool     createVisit(const visit& Visit) { return createVisit(m_dbController, Visit); }
        inline bool     updateVisit(const visit& Visit) { return updateVisit(m_dbController, Visit); }
        inline bool     deleteVisit(uint32_t id)        { return deleteVisit(m_dbController, id);   }
    private:
        DBController* m_dbController;
    };
}
