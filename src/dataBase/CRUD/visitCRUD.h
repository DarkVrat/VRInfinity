#pragma once

#include <vector>
#include "../tables/visit.h"
#include "../DBController.h"

namespace DB
{
    class VisitsCRUD
    {
    public:
        VisitsCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<visit> getAllVisitsByEmail(DBController* dbController, const std::string& email)
        {
            std::vector<visit> visits;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, email, start, end, service FROM visit WHERE email = ?;"));
                stmt.bind(0, email.c_str());
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits.push_back(visit(results.get<uint32_t>(0),           // id
                        results.get<std::string>(1),        // name
                        results.get<std::string>(2),        // surname
                        results.get<std::string>(3),        // email
                        results.get<std::string>(4),        // start
                        results.get<std::string>(5),        // end
                        results.get<std::string>(6)));      // service
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
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, email, start, end, service FROM visit;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits.push_back(visit( results.get<uint32_t>(0),           // id
                                            results.get<std::string>(1),        // name
                                            results.get<std::string>(2),        // surname
                                            results.get<std::string>(3),        // email
                                            results.get<std::string>(4),        // start
                                            results.get<std::string>(5),        // end
                                            results.get<std::string>(6)));      // service
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
                nanodbc::statement stmt(dbController->statement("SELECT id, name, surname, email, start, end, service FROM visit WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    Visit.setID(results.get<uint32_t>(0));
                    Visit.setName(results.get<std::string>(1));
                    Visit.setSurname(results.get<std::string>(2));
                    Visit.setEmail(results.get<std::string>(3));
                    Visit.setStart(results.get<std::string>(4));
                    Visit.setEnd(results.get<std::string>(5));
                    Visit.setService(results.get<std::string>(6));
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
                nanodbc::statement stmt(dbController->statement("INSERT INTO visit (name, surname, email, start, end, service) VALUES (?, ?, ?, ?, ?, ?);"));
                stmt.bind(0, Visit.getName().c_str());
                stmt.bind(1, Visit.getSurname().c_str());
                stmt.bind(2, Visit.getEmail().c_str());
                stmt.bind(3, Visit.getStart().c_str());
                stmt.bind(4, Visit.getEnd().c_str());
                stmt.bind(5, Visit.getService().c_str());
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
                nanodbc::statement stmt(dbController->statement("UPDATE visit SET name = ?, surname = ?, email = ?, start = ?, end = ?, service = ? WHERE id = ?;"));
                stmt.bind(0, Visit.getName().c_str());
                stmt.bind(1, Visit.getSurname().c_str());
                stmt.bind(2, Visit.getEmail().c_str());
                stmt.bind(3, Visit.getStart().c_str());
                stmt.bind(4, Visit.getEnd().c_str());
                stmt.bind(5, Visit.getService().c_str());
                stmt.bind(6, &Visit.getID());
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

        inline std::vector<visit> getAllVisitsByEmail(const std::string& email) { return getAllVisitsByEmail(m_dbController, email); }
        inline std::vector<visit> getAllVisits()        { return getAllVisits(m_dbController);      }
        inline visit    getVisitByID(uint32_t id)       { return getVisitByID(m_dbController, id);  }
        inline bool     createVisit(const visit& Visit) { return createVisit(m_dbController, Visit); }
        inline bool     updateVisit(const visit& Visit) { return updateVisit(m_dbController, Visit); }
        inline bool     deleteVisit(uint32_t id)        { return deleteVisit(m_dbController, id);   }
    private:
        DBController* m_dbController;
    };
}
