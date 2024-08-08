#pragma once

#include <vector>
#include "../tables/game_vr.h"
#include "../DBController.h"

namespace DB
{
    class game_vr_CRUD
    {
    public:
        game_vr_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<game_vr> getAllGames(DBController* dbController)
        {
            std::vector<game_vr> games;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, text, image FROM game_vr;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    game_vr Game;
                    Game.setId(results.get<uint32_t>(0));
                    Game.setName(results.get<std::string>(1));
                    Game.setText(results.get<std::string>(2));
                    Game.setImage(results.get<std::string>(3));

                    games.push_back(std::move(Game));  
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return games;
        }

        static game_vr getGameByID(DBController* dbController, uint32_t id)
        {
            game_vr Game;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, text, image FROM game_vr WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    Game.setId(results.get<uint32_t>(0));
                    Game.setName(results.get<std::string>(1));
                    Game.setText(results.get<std::string>(2));
                    Game.setImage(results.get<std::string>(3));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return Game;
        }

        static uint32_t createGame(DBController* dbController, const game_vr& Game)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO game_vr (name, text, image) VALUES (?, ?, ?);"));
                stmt.bind(0, Game.getName().c_str());
                stmt.bind(1, Game.getText().c_str());
                stmt.bind(2, Game.getImage().c_str());
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

        static bool updateGame(DBController* dbController, const game_vr& Game)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE game_vr SET name = ?, text = ?, image = ? WHERE id = ?;"));
                stmt.bind(0, Game.getName().c_str());
                stmt.bind(1, Game.getText().c_str());
                stmt.bind(2, Game.getImage().c_str());
                stmt.bind(3, &Game.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteGame(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM game_vr WHERE id = ?;"));
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

        inline std::vector<game_vr> getAllGames()       { return getAllGames(m_dbController);       }
        inline game_vr  getGameByID(uint32_t id)        { return getGameByID(m_dbController, id);   }
        inline uint32_t createGame(const game_vr& Game) { return createGame(m_dbController, Game);  }
        inline bool     updateGame(const game_vr& Game) { return updateGame(m_dbController, Game);  }
        inline bool     deleteGame(uint32_t id)         { return deleteGame(m_dbController, id);    }
    private:
        DBController* m_dbController;
    };
}
