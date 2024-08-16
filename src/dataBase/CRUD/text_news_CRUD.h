#pragma once

#include <vector>
#include "../tables/text_news.h"
#include "../DBController.h"

namespace DB
{
    class text_news_CRUD
    {
    public:
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

        static uint32_t createTextNews(DBController* dbController, const text_news& TextNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO text_news (id, text, next_id) VALUES (?, ?, ?);"));
                stmt.bind(0, &TextNews.getId());
                stmt.bind(1, TextNews.getText().c_str());
                stmt.bind(2, &TextNews.getNextId());
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
    };
}
