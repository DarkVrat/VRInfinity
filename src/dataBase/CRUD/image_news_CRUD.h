#pragma once

#include <vector>
#include "../tables/image_news.h"
#include "../DBController.h"

namespace DB
{
    class image_news_CRUD
    {
    public:
        image_news_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<image_news> getAllImageNewses(DBController* dbController)
        {
            std::vector<image_news> imageNewses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, image, next_id FROM image_news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    image_news ImageNews;
                    ImageNews.setId(results.get<int32_t>(0));
                    ImageNews.setImage(results.get<std::string>(1));
                    ImageNews.setNextId(results.get<int32_t>(2));

                    imageNewses.push_back(std::move(ImageNews));      
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return imageNewses;
        }

        static image_news getImageNewsByID(DBController* dbController, int32_t id)
        {
            image_news ImageNews;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, image, next_id FROM image_news WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    ImageNews.setId(results.get<int32_t>(0));
                    ImageNews.setImage(results.get<std::string>(1));
                    ImageNews.setNextId(results.get<int32_t>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return ImageNews;
        }

        static uint32_t createImageNews(DBController* dbController, const image_news& ImageNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO image_news (image, next_id) VALUES (?, ?);"));
                stmt.bind(0, ImageNews.getImage().c_str());
                stmt.bind(1, &ImageNews.getNextId());
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

        static bool updateImageNews(DBController* dbController, const image_news& ImageNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE image_news SET image = ?, next_id = ? WHERE id = ?;"));
                stmt.bind(0, ImageNews.getImage().c_str());
                stmt.bind(1, &ImageNews.getNextId());
                stmt.bind(2, &ImageNews.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteImageNews(DBController* dbController, int32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM image_news WHERE id = ?;"));
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

        inline std::vector<image_news> getAllImageNewses()              { return getAllImageNewses(m_dbController);             }
        inline image_news     getImageNewsByID(int32_t id)              { return getImageNewsByID(m_dbController, id);          }
        inline uint32_t createImageNews(const image_news& ImageNews)    { return createImageNews(m_dbController, ImageNews);    }
        inline bool     updateImageNews(const image_news& ImageNews)    { return updateImageNews(m_dbController, ImageNews);    }
        inline bool     deleteImageNews(int32_t id)                     { return deleteImageNews(m_dbController, id);           }
    private:
        DBController* m_dbController;
    };
}
