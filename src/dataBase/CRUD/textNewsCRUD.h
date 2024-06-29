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

        static std::vector<textNews> getAllTextNewses(DBController* dbController)
        {
            std::vector<textNews> textNewses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, text, next_id FROM text_news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) { 
                    textNews TextNews;
                    TextNews.setID(results.get<uint32_t>(0));
                    TextNews.setText(results.get<std::string>(1));
                    TextNews.setNextID(results.get<uint32_t>(2));

                    textNewses.push_back(std::move(TextNews));     // next_id
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return textNewses;
        }

        static textNews getTextNewsByID(DBController* dbController, uint32_t id)
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

        static bool createTextNews(DBController* dbController, const textNews& TextNews)
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

        static bool updateTextNews(DBController* dbController, const textNews& TextNews)
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

        static bool deleteTextNews(DBController* dbController, uint32_t id)
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

        inline std::vector<textNews> getAllTextNewses()             { return getAllTextNewses(m_dbController);          }
        inline textNews     getTextNewsByID(uint32_t id)            { return getTextNewsByID(m_dbController, id);       }
        inline bool     createTextNews(const textNews& TextNews)    { return createTextNews(m_dbController, TextNews);  }
        inline bool     updateTextNews(const textNews& TextNews)    { return updateTextNews(m_dbController, TextNews);  }
        inline bool     deleteTextNews(uint32_t id)                 { return deleteTextNews(m_dbController, id);        }
    private:
        DBController* m_dbController;
    };
}
