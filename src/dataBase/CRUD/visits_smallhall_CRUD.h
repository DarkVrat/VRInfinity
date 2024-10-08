#pragma once

#include <vector>
#include "../tables/visits_smallhall.h"
#include "../DBController.h"

namespace DB
{
    class visits_smallhall_CRUD
    {
    public:
        static std::vector<visits_smallhall> getVisitsSmallhallInRange(DBController* dbController, const std::string& startDate, const std::string& endDate)
        {
            std::vector<visits_smallhall> visitsSmallhallVec;
            try
            {
                std::string query = "SELECT id, start, end, visit_id FROM visits_smallhall "
                    "WHERE start >= ? AND end <= ?;";
                nanodbc::statement stmt(dbController->statement(query));

                stmt.bind(0, startDate.c_str());
                stmt.bind(1, endDate.c_str());

                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits_smallhall VisitsSmallhall;
                    VisitsSmallhall.setId(results.get<uint32_t>(0));
                    VisitsSmallhall.setStart(results.get<std::string>(1));
                    VisitsSmallhall.setEnd(results.get<std::string>(2));
                    VisitsSmallhall.setVisitId(results.get<uint32_t>(3));

                    visitsSmallhallVec.push_back(std::move(VisitsSmallhall));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visitsSmallhallVec;
        }

        static std::vector<visits_smallhall> getAllVisitsSmallhall(DBController* dbController)
        {
            std::vector<visits_smallhall> visitsSmallhallVec;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, visit_id FROM visits_smallhall;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    visits_smallhall VisitsSmallhall;
                    VisitsSmallhall.setId(results.get<uint32_t>(0));
                    VisitsSmallhall.setStart(results.get<std::string>(1));
                    VisitsSmallhall.setEnd(results.get<std::string>(2));
                    VisitsSmallhall.setVisitId(results.get<uint32_t>(3));

                    visitsSmallhallVec.push_back(std::move(VisitsSmallhall));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return visitsSmallhallVec;
        }

        static visits_smallhall getVisitsSmallhallByID(DBController* dbController, uint32_t id)
        {
            visits_smallhall VisitsSmallhall;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, start, end, visit_id FROM visits_smallhall WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    VisitsSmallhall.setId(results.get<uint32_t>(0));
                    VisitsSmallhall.setStart(results.get<std::string>(1));
                    VisitsSmallhall.setEnd(results.get<std::string>(2));
                    VisitsSmallhall.setVisitId(results.get<uint32_t>(3));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return VisitsSmallhall;
        }

        static uint32_t createVisitsSmallhall(DBController* dbController, const visits_smallhall& VisitsSmallhall)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO visits_smallhall (start, end, visit_id) VALUES (?, ?, ?);"));
                stmt.bind(0, VisitsSmallhall.getStart().c_str());
                stmt.bind(1, VisitsSmallhall.getEnd().c_str());
                stmt.bind(2, &VisitsSmallhall.getVisitId());
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

        static bool updateVisitsSmallhall(DBController* dbController, const visits_smallhall& VisitsSmallhall)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE visits_smallhall SET start = ?, end = ?, visit_id = ? WHERE id = ?;"));
                stmt.bind(0, VisitsSmallhall.getStart().c_str());
                stmt.bind(1, VisitsSmallhall.getEnd().c_str());
                stmt.bind(2, &VisitsSmallhall.getVisitId());
                stmt.bind(3, &VisitsSmallhall.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteVisitsSmallhall(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM visits_smallhall WHERE id = ?;"));
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
