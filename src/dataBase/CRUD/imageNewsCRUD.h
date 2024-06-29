#pragma once

#include <vector>
#include "../tables/imageNews.h"
#include "../DBController.h"

namespace DB
{
    class imageNewsCRUD
    {
    public:
        imageNewsCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<imageNews> getAllImageNewses(DBController* dbController)
        {
            std::vector<imageNews> imageNewses;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, image, next_id FROM image_news;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    imageNews ImageNews;
                    ImageNews.setID(results.get<int32_t>(0));
                    ImageNews.setImage(results.get<std::string>(1));
                    ImageNews.setNextID(results.get<int32_t>(2));

                    imageNewses.push_back(std::move(ImageNews));      // next_id
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return imageNewses;
        }

        static imageNews getImageNewsByID(DBController* dbController, int32_t id)
        {
            imageNews ImageNews;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, image, next_id FROM image_news WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    ImageNews.setID(results.get<int32_t>(0));
                    ImageNews.setImage(results.get<std::string>(1));
                    ImageNews.setNextID(results.get<int32_t>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return ImageNews;
        }

        static bool createImageNews(DBController* dbController, const imageNews& ImageNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO image_news (image, next_id) VALUES (?, ?);"));
                stmt.bind(0, ImageNews.getImage().c_str());
                stmt.bind(1, &ImageNews.getNextID());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateImageNews(DBController* dbController, const imageNews& ImageNews)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE image_news SET image = ?, next_id = ? WHERE id = ?;"));
                stmt.bind(0, ImageNews.getImage().c_str());
                stmt.bind(1, &ImageNews.getNextID());
                stmt.bind(2, &ImageNews.getID());
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

        inline std::vector<imageNews> getAllImageNewses()           { return getAllImageNewses(m_dbController);             }
        inline imageNews     getImageNewsByID(int32_t id)           { return getImageNewsByID(m_dbController, id);          }
        inline bool     createImageNews(const imageNews& ImageNews) { return createImageNews(m_dbController, ImageNews);    }
        inline bool     updateImageNews(const imageNews& ImageNews) { return updateImageNews(m_dbController, ImageNews);    }
        inline bool     deleteImageNews(int32_t id)                 { return deleteImageNews(m_dbController, id);           }
    private:
        DBController* m_dbController;
    };
}
