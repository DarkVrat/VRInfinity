#pragma once

#include <vector>
#include "../tables/game.h"
#include "../DBController.h"

namespace DB
{
    class gameCRUD
    {
    public:
        gameCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<game> getAllGames(DBController* dbController)
        {
            std::vector<game> games;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, text, image FROM game;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    games.push_back(game(   results.get<uint32_t>(0),       // id
                                            results.get<std::string>(1),    // name
                                            results.get<std::string>(2),    // text
                                            results.get<std::string>(3)));  // image
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return games;
        }

        static game getGameByID(DBController* dbController, uint32_t id)
        {
            game Game;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, name, text, image FROM game WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    Game.setID(results.get<uint32_t>(0));
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

        static bool createGame(DBController* dbController, const game& Game)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO game (name, text, image) VALUES (?, ?, ?);"));
                stmt.bind(0, Game.getName().c_str());
                stmt.bind(1, Game.getText().c_str());
                stmt.bind(2, Game.getImage().c_str());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateGame(DBController* dbController, const game& Game)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE game SET name = ?, text = ?, image = ? WHERE id = ?;"));
                stmt.bind(0, Game.getName().c_str());
                stmt.bind(1, Game.getText().c_str());
                stmt.bind(2, Game.getImage().c_str());
                stmt.bind(3, &Game.getID());
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
                nanodbc::statement stmt(dbController->statement("DELETE FROM game WHERE id = ?;"));
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

        inline std::vector<game> getAllGames()          { return getAllGames(m_dbController);       }
        inline game     getGameByID(uint32_t id)        { return getGameByID(m_dbController, id);   }
        inline bool     createGame(const game& Game)    { return createGame(m_dbController, Game);  }
        inline bool     updateGame(const game& Game)    { return updateGame(m_dbController, Game);  }
        inline bool     deleteGame(uint32_t id)         { return deleteGame(m_dbController, id);    }
    private:
        DBController* m_dbController;
    };
}
