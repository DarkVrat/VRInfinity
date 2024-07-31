#pragma once

#include <vector>
#include "../tables/reset_password.h"
#include "../DBController.h"

namespace DB
{
    class reset_password_CRUD
    {
    public:
        reset_password_CRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<reset_password> getAllResetPasswordes(DBController* dbController)
        {
            std::vector<reset_password> resetPasswordes;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, phone, new_password FROM reset_password;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    reset_password ResetPassword;
                    ResetPassword.setId(results.get<uint32_t>(0));
                    ResetPassword.setPhone(results.get<std::string>(1));
                    ResetPassword.setNewPassword(results.get<std::string>(2));

                    resetPasswordes.push_back(std::move(ResetPassword));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return resetPasswordes;
        }

        static reset_password getResetPasswordByID(DBController* dbController, uint32_t id)
        {
            reset_password ResetPassword;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, phone, new_password FROM reset_password WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    ResetPassword.setId(results.get<uint32_t>(0));
                    ResetPassword.setPhone(results.get<std::string>(1));
                    ResetPassword.setNewPassword(results.get<std::string>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return ResetPassword;
        }

        static bool createResetPassword(DBController* dbController, const reset_password& ResetPassword)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO reset_password (phone, new_password) VALUES (?, ?);"));
                stmt.bind(0, ResetPassword.getPhone().c_str());
                stmt.bind(1, ResetPassword.getNewPassword().c_str());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool updateResetPassword(DBController* dbController, const reset_password& ResetPassword)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE reset_password SET phone = ?, new_password = ? WHERE id = ?;"));
                stmt.bind(0, ResetPassword.getPhone().c_str());
                stmt.bind(1, ResetPassword.getNewPassword().c_str());
                stmt.bind(2, &ResetPassword.getId());
                nanodbc::execute(stmt);
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        static bool deleteResetPassword(DBController* dbController, uint32_t id)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("DELETE FROM reset_password WHERE id = ?;"));
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

        inline std::vector<reset_password>  getAllResetPasswordes()                                     { return getAllResetPasswordes(m_dbController);                 }
        inline reset_password               getResetPasswordByID(uint32_t id)                           { return getResetPasswordByID(m_dbController, id);              }
        inline bool                         createResetPassword(const reset_password& ResetPassword)    { return createResetPassword(m_dbController, ResetPassword);    }
        inline bool                         updateResetPassword(const reset_password& ResetPassword)    { return updateResetPassword(m_dbController, ResetPassword);    }
        inline bool                         deleteResetPassword(uint32_t id)                            { return deleteResetPassword(m_dbController, id);               }
    private:
        DBController* m_dbController;
    };
}
