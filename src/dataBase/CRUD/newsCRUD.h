#pragma once

#include <vector>
#include "../tables/news.h"
#include "../DBController.h"

namespace DB
{
    class newsCRUD
    {
    public:
        newsCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<news> getAllNewses(DBController* dbController)
        {
            std::vector<news> newses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, time, subject, text_news_id, image_news_id FROM news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    news News;
                    News.setID(results.get<uint32_t>(0));
                    News.setTime(results.get<std::string>(1));
                    News.setSubject(results.get<std::string>(2));
                    News.setTextNewsID(results.get<uint32_t>(3));
                    News.setImageNewsID(results.get<int32_t>(4));

                    newses.push_back(std::move(News));          // image_news_id
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
                    News.setID(results.get<uint32_t>(0));
                    News.setTime(results.get<std::string>(1));
                    News.setSubject(results.get<std::string>(2));
                    News.setTextNewsID(results.get<uint32_t>(3));
                    News.setImageNewsID(results.get<int32_t>(4));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return News;
        }

        static bool createNews(DBController* dbController, const news& News)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO news (time, subject, text_news_id, image_news_id) VALUES (?, ?, ?, ?);"));
                stmt.bind(0, News.getTime().c_str());
                stmt.bind(1, News.getSubject().c_str());
                stmt.bind(2, &News.getTextNewsID());
                stmt.bind(3, &News.getImageNewsID());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateNews(DBController* dbController, const news& News)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE news SET time = ?, subject = ?, text_news_id = ?, image_news_id = ? WHERE id = ?;"));
                stmt.bind(0, News.getTime().c_str());
                stmt.bind(1, News.getSubject().c_str());
                stmt.bind(2, &News.getTextNewsID());
                stmt.bind(3, &News.getImageNewsID());
                stmt.bind(4, &News.getID());
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

        inline std::vector<news> getAllNewses()         { return getAllNewses(m_dbController);      }
        inline news     getNewsByID(uint32_t id)        { return getNewsByID(m_dbController, id);   }
        inline bool     createNews(const news& News)    { return createNews(m_dbController, News);  }
        inline bool     updateNews(const news& News)    { return updateNews(m_dbController, News);  }
        inline bool     deleteNews(uint32_t id)         { return deleteNews(m_dbController, id);    }
    private:
        DBController* m_dbController;
    };
}
