#pragma once

#include <vector>
#include "visits.h"
#include "DBController.h"

namespace DB
{
    class VisitsCRUD
    {
    public:
        VisitsCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<Visits> getAllVisits(DBController* dbController)
        {
            std::vector<Visits> visits;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, users_id FROM visits"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits.push_back(Visits(results.get<int>(0),        // id
                                            results.get<std::string>(1),// start
                                            results.get<std::string>(2),// end
                                            results.get<int>(3)));      // users_id
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visits;
        }

        static Visits getVisitByID(DBController* dbController, uint32_t id)
        {
            Visits visit;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, users_id FROM visits WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    visit.setID(results.get<int>(0));
                    visit.setStart(results.get<std::string>(1));
                    visit.setEnd(results.get<std::string>(2));
                    visit.setIDUsers(results.get<int>(3));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visit;
        }

        static bool createVisit(DBController* dbController, const Visits& visit)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO visits (start, end, users_id) VALUES (?, ?, ?);"));
                stmt.bind(0, visit.getStart().c_str());
                stmt.bind(1, visit.getEnd().c_str());
                stmt.bind(2, &visit.getIDUsers());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateVisit(DBController* dbController, const Visits& visit)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE visits SET start = ?, end = ?, users_id = ? WHERE id = ?;"));
                stmt.bind(0, visit.getStart().c_str());
                stmt.bind(1, visit.getEnd().c_str());
                stmt.bind(2, &visit.getIDUsers());
                stmt.bind(3, &visit.getID());
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
                nanodbc::statement stmt(dbController->statement("DELETE FROM visits WHERE id = ?;"));
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

        inline std::vector<Visits> getAllVisits()       { return getAllVisits(m_dbController);      }
        inline Visits   getVisitByID(uint32_t id)       { return getVisitByID(m_dbController, id);  }
        inline bool     createVisit(const Visits& user) { return createVisit(m_dbController, user); }
        inline bool     updateVisit(const Visits& user) { return updateVisit(m_dbController, user); }
        inline bool     deleteVisit(uint32_t id)        { return deleteVisit(m_dbController, id);   }
    private:
        DBController* m_dbController;
    };
}
