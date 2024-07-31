#pragma once

#include <vector>
#include "../tables/text_news.h"
#include "../DBController.h"

namespace DB
{
    class text_news_CRUD
    {
    public:
        text_news_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<text_news> getAllTextNewses(DBController* dbController)
        {
            std::vector<text_news> textNewses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, text, next_id FROM text_news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) { 
                    text_news TextNews;
                    TextNews.setId(results.get<uint32_t>(0));
                    TextNews.setText(results.get<std::string>(1));
                    TextNews.setNextId(results.get<uint32_t>(2));

                    textNewses.push_back(std::move(TextNews));    
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return textNewses;
        }

        static text_news getTextNewsByID(DBController* dbController, uint32_t id)
        {
            text_news TextNews;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, text, next_id FROM text_news WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    TextNews.setId(results.get<uint32_t>(0));
                    TextNews.setText(results.get<std::string>(1));
                    TextNews.setNextId(results.get<uint32_t>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return TextNews;
        }

        static bool createTextNews(DBController* dbController, const text_news& TextNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO text_news (text, next_id) VALUES (?, ?);"));
                stmt.bind(0, TextNews.getText().c_str());
                stmt.bind(1, &TextNews.getNextId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateTextNews(DBController* dbController, const text_news& TextNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE text_news SET text = ?, next_id = ? WHERE id = ?;"));
                stmt.bind(0, TextNews.getText().c_str());
                stmt.bind(1, &TextNews.getNextId());
                stmt.bind(2, &TextNews.getId());
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

        inline std::vector<text_news> getAllTextNewses()            { return getAllTextNewses(m_dbController);          }
        inline text_news     getTextNewsByID(uint32_t id)           { return getTextNewsByID(m_dbController, id);       }
        inline bool     createTextNews(const text_news& TextNews)   { return createTextNews(m_dbController, TextNews);  }
        inline bool     updateTextNews(const text_news& TextNews)   { return updateTextNews(m_dbController, TextNews);  }
        inline bool     deleteTextNews(uint32_t id)                 { return deleteTextNews(m_dbController, id);        }
    private:
        DBController* m_dbController;
    };
}
