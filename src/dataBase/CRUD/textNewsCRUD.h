#pragma once

#include <vector>
#include "../tables/textNews.h"
#include "../DBController.h"

namespace DB
{
    class textNewsCRUD
    {
    public:
        textNewsCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<textNews> getAlltextNewses(DBController* dbController)
        {
            std::vector<textNews> textNewses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, text, next_id FROM text_news"));
                nanodbc::result results = execute(stmt);

                while (results.next()) { 
                    textNewses.push_back(textNews(  results.get<uint32_t>(0),       // id
                                                    results.get<std::string>(1),    // text
                                                    results.get<uint32_t>(2)));     // next_id
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return textNewses;
        }

        static textNews gettextNewsByID(DBController* dbController, uint32_t id)
        {
            textNews TextNews;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, text, next_id FROM text_news WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    TextNews.setID(results.get<uint32_t>(0));
                    TextNews.setText(results.get<std::string>(1));
                    TextNews.setNextID(results.get<uint32_t>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return TextNews;
        }

        static bool createtextNews(DBController* dbController, const textNews& TextNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO text_news (text, next_id) VALUES (?, ?);"));
                stmt.bind(0, TextNews.getText().c_str());
                stmt.bind(1, &TextNews.getNextID());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updatetextNews(DBController* dbController, const textNews& TextNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE text_news SET text = ?, next_id = ? WHERE id = ?;"));
                stmt.bind(0, TextNews.getText().c_str());
                stmt.bind(1, &TextNews.getNextID());
                stmt.bind(2, &TextNews.getID());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deletetextNews(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM text_news WHERE id = ?;"));
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

        inline std::vector<textNews> getAlltextNewses()             { return getAlltextNewses(m_dbController);          }
        inline textNews     gettextNewsByID(uint32_t id)            { return gettextNewsByID(m_dbController, id);       }
        inline bool     createtextNews(const textNews& TextNews)    { return createtextNews(m_dbController, TextNews);  }
        inline bool     updatetextNews(const textNews& TextNews)    { return updatetextNews(m_dbController, TextNews);  }
        inline bool     deletetextNews(uint32_t id)                 { return deletetextNews(m_dbController, id);        }
    private:
        DBController* m_dbController;
    };
}
