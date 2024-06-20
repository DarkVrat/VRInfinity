#pragma once

#include <vector>
#include "users.h"
#include "DBController.h"

namespace DB
{
	class UsersCRUD
	{
	public:
        UsersCRUD(DBController* dbController) :m_dbController(dbController) {}
          
        static std::vector<Users> getAllUsers(DBController* dbController)
        {
            std::vector<Users> users;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, email, password_hash, name, surname, role FROM users"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    users.push_back(Users(  results.get<int>(0),            // id
                                            results.get<std::string>(1),    // email
                                            results.get<std::string>(2),    // password_hash
                                            results.get<std::string>(3),    // name
                                            results.get<std::string>(4),    // surname
                                            results.get<std::string>(5)));  // role
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return users;
        }

        static Users getUserByID(DBController* dbController, uint32_t id)
        {
            Users user;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, email, password_hash, name, surname, role FROM users WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    user.setID(results.get<int>(0));
                    user.setEmail(results.get<std::string>(1));
                    user.setPasswordHash(results.get<std::string>(2));
                    user.setName(results.get<std::string>(3));
                    user.setSurname(results.get<std::string>(4));
                    user.setRole(results.get<std::string>(5));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return user;
        }

        static bool createUser(DBController* dbController, const Users& user)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO users (email, password_hash, name, surname, role) VALUES (?, ?, ?, ?, ?);"));
                stmt.bind(0, user.getEmail().c_str());
                stmt.bind(1, user.getPasswordHash().c_str());
                stmt.bind(2, user.getName().c_str());
                stmt.bind(3, user.getSurname().c_str());
                stmt.bind(4, user.getRole().c_str());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateUser(DBController* dbController, const Users& user)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE users SET email = ?, password_hash = ?, name = ?, surname = ?, role = ? WHERE id = ?;"));
                stmt.bind(0, user.getEmail().c_str());
                stmt.bind(1, user.getPasswordHash().c_str());
                stmt.bind(2, user.getName().c_str());
                stmt.bind(3, user.getSurname().c_str());
                stmt.bind(4, user.getRole().c_str());
                stmt.bind(5, &user.getID());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteUser(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM users WHERE id = ?;"));
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

        inline std::vector<Users> getAllUsers()         { return getAllUsers(m_dbController); }
        inline Users    getUserByID(uint32_t id)        { return getUserByID(m_dbController, id); }
        inline bool     createUser(const Users& user)   { return createUser(m_dbController, user); }
        inline bool     updateUser(const Users& user)   { return updateUser(m_dbController, user); }
        inline bool     deleteUser(uint32_t id)         { return deleteUser(m_dbController, id); }
    private:
        DBController* m_dbController;
	};
}