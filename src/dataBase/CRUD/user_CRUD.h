#pragma once

#include <vector>
#include "../tables/user.h"
#include "../DBController.h"

namespace DB
{
	class user_CRUD
	{
	public:
        static std::vector<user> getAllUsers(DBController* dbController)
        {
            std::vector<user> users;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, phone, password_hash, name, surname, role FROM user;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    user User;
                    User.setId(results.get<uint32_t>(0));
                    User.setPhone(results.get<std::string>(1));
                    User.setPasswordHash(results.get<std::string>(2));
                    User.setName(results.get<std::string>(3));
                    User.setSurname(results.get<std::string>(4));
                    User.setRole(results.get<std::string>(5));

                    users.push_back(std::move(User));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return users;
        }

        static user getUserByID(DBController* dbController, uint32_t id)
        {
            user User;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, phone, password_hash, name, surname, role FROM user WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    User.setId(results.get<uint32_t>(0));
                    User.setPhone(results.get<std::string>(1));
                    User.setPasswordHash(results.get<std::string>(2));
                    User.setName(results.get<std::string>(3));
                    User.setSurname(results.get<std::string>(4));
                    User.setRole(results.get<std::string>(5));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return User;
        }

        static user getUserByPhone(DBController* dbController, const std::string& phone)
        {
            user User;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, phone, password_hash, name, surname, role FROM user WHERE phone = ?;"));
                stmt.bind(0, phone.c_str());
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    User.setId(results.get<uint32_t>(0));
                    User.setPhone(results.get<std::string>(1));
                    User.setPasswordHash(results.get<std::string>(2));
                    User.setName(results.get<std::string>(3));
                    User.setSurname(results.get<std::string>(4));
                    User.setRole(results.get<std::string>(5));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return User;
        }

        static uint32_t createUser(DBController* dbController, const user& User)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO user (phone, password_hash, name, surname, role) VALUES (?, ?, ?, ?, ?);"));
                stmt.bind(0, User.getPhone().c_str());
                stmt.bind(1, User.getPasswordHash().c_str());
                stmt.bind(2, User.getName().c_str());
                stmt.bind(3, User.getSurname().c_str());
                stmt.bind(4, User.getRole().c_str());
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

        static bool updateUser(DBController* dbController, const user& User)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE user SET phone = ?, password_hash = ?, name = ?, surname = ?, role = ? WHERE id = ?;"));
                stmt.bind(0, User.getPhone().c_str());
                stmt.bind(1, User.getPasswordHash().c_str());
                stmt.bind(2, User.getName().c_str());
                stmt.bind(3, User.getSurname().c_str());
                stmt.bind(4, User.getRole().c_str());
                stmt.bind(5, &User.getId());
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
                nanodbc::statement stmt(dbController->statement("DELETE FROM user WHERE id = ?;"));
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
