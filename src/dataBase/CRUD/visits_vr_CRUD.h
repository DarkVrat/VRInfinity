#pragma once

#include <vector>
#include "../tables/visits_vr.h"
#include "../DBController.h"

namespace DB
{
    class visits_vr_CRUD
    {
    public:
        visits_vr_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<visits_vr> getAllVisitsVr(DBController* dbController)
        {
            std::vector<visits_vr> visitsVrVec;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, visit_id FROM visits_vr;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits_vr VisitsVr;
                    VisitsVr.setId(results.get<uint32_t>(0));
                    VisitsVr.setStart(results.get<std::string>(1));
                    VisitsVr.setEnd(results.get<std::string>(2));
                    VisitsVr.setVisitId(results.get<uint32_t>(3));

                    visitsVrVec.push_back(std::move(VisitsVr));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visitsVrVec;
        }

        static visits_vr getVisitsVrByID(DBController* dbController, uint32_t id)
        {
            visits_vr VisitsVr;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, visit_id FROM visits_vr WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    VisitsVr.setId(results.get<uint32_t>(0));
                    VisitsVr.setStart(results.get<std::string>(1));
                    VisitsVr.setEnd(results.get<std::string>(2));
                    VisitsVr.setVisitId(results.get<uint32_t>(3));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return VisitsVr;
        }

        static bool createVisitsVr(DBController* dbController, const visits_vr& VisitsVr)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO visits_vr (start, end, visit_id) VALUES (?, ?);"));
                stmt.bind(0, VisitsVr.getStart().c_str());
                stmt.bind(1, VisitsVr.getEnd().c_str());
                stmt.bind(2, &VisitsVr.getVisitId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateVisitsVr(DBController* dbController, const visits_vr& VisitsVr)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE visits_vr SET start = ?, end = ?, visit_id = ? WHERE id = ?;"));
                stmt.bind(0, VisitsVr.getStart().c_str());
                stmt.bind(1, VisitsVr.getEnd().c_str());
                stmt.bind(2, &VisitsVr.getVisitId());
                stmt.bind(3, &VisitsVr.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteVisitsVr(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM visits_vr WHERE id = ?;"));
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

        inline std::vector<visits_vr> getAllVisitsVr()              { return getAllVisitsVr(m_dbController);            }  
        inline visits_vr  getVisitsVrByID(uint32_t id)              { return getVisitsVrByID(m_dbController, id);       }
        inline bool     createVisitsVr(const visits_vr& VisitsVr)   { return createVisitsVr(m_dbController, VisitsVr);  }
        inline bool     updateVisitsVr(const visits_vr& VisitsVr)   { return updateVisitsVr(m_dbController, VisitsVr);  }
        inline bool     deleteVisitsVr(uint32_t id)                 { return deleteVisitsVr(m_dbController, id);        }
    private:
        DBController* m_dbController;
    };
}
