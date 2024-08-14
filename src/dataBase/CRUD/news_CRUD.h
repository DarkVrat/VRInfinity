#pragma once

#include <vector>
#include "../tables/news.h"
#include "../DBController.h"

namespace DB
{
    class news_CRUD
    {
    public:
        static std::vector<news> getAllNewses(DBController* dbController)
        {
            std::vector<news> newses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, time, subject, text_news_id, image_news_id FROM news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    news News;
                    News.setId(results.get<uint32_t>(0));
                    News.setTime(results.get<std::string>(1));
                    News.setSubject(results.get<std::string>(2));
                    News.setTextNewsId(results.get<uint32_t>(3));
                    News.setImageNewsId(results.get<uint32_t>(4));

                    newses.push_back(std::move(News));          
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return newses;
        }

        static news getNewsByID(DBController* dbController, uint32_t id)
        {
            news News;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, time, subject, text_news_id, image_news_id FROM news WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    News.setId(results.get<uint32_t>(0));
                    News.setTime(results.get<std::string>(1));
                    News.setSubject(results.get<std::string>(2));
                    News.setTextNewsId(results.get<uint32_t>(3));
                    News.setImageNewsId(results.get<uint32_t>(4));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return News;
        }

        static uint32_t createNews(DBController* dbController, const news& News)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO news (id, time, subject, text_news_id, image_news_id) VALUES (?, ?, ?, ?, ?);"));
                stmt.bind(0, &News.getId());
                stmt.bind(1, News.getTime().c_str());
                stmt.bind(2, News.getSubject().c_str());
                stmt.bind(3, &News.getTextNewsId());
                stmt.bind(4, &News.getImageNewsId());
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

        static bool updateNews(DBController* dbController, const news& News)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE news SET time = ?, subject = ?, text_news_id = ?, image_news_id = ? WHERE id = ?;"));
                stmt.bind(0, News.getTime().c_str());
                stmt.bind(1, News.getSubject().c_str());
                stmt.bind(2, &News.getTextNewsId());
                stmt.bind(3, &News.getImageNewsId());
                stmt.bind(4, &News.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteNews(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM news WHERE id = ?;"));
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
