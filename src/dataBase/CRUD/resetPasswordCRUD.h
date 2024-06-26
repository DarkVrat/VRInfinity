#pragma once

#include <vector>
#include "../tables/resetPassword.h"
#include "../DBController.h"

namespace DB
{
    class resetPasswordCRUD
    {
    public:
        resetPasswordCRUD(DBController* dbController) :m_dbController(dbController) {}

        static std::vector<resetPassword> getAllResetPasswordes(DBController* dbController)
        {
            std::vector<resetPassword> resetPasswordes;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, email, new_password FROM reset_password;"));
                nanodbc::result results = execute(stmt);

                while (results.next()) {
                    resetPassword ResetPassword;
                    ResetPassword.setID(results.get<uint32_t>(0));
                    ResetPassword.setEmail(results.get<std::string>(1));
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

        static resetPassword getResetPasswordByID(DBController* dbController, uint32_t id)
        {
            resetPassword ResetPassword;
            try
            {
                nanodbc::statement stmt(dbController->statement("SELECT id, email, new_password FROM reset_password WHERE id = ?;"));
                stmt.bind(0, &id);
                nanodbc::result results = execute(stmt);

                if (results.next()) {
                    ResetPassword.setID(results.get<uint32_t>(0));
                    ResetPassword.setEmail(results.get<std::string>(1));
                    ResetPassword.setNewPassword(results.get<std::string>(2));
                }
            }
            catch (const nanodbc::database_error& e)
            {
                std::cerr << "Database error: " << e.what() << std::endl;
            }
            return ResetPassword;
        }

        static bool createResetPassword(DBController* dbController, const resetPassword& ResetPassword)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("INSERT INTO reset_password (email, new_password) VALUES (?, ?);"));
                stmt.bind(0, ResetPassword.getEmail().c_str());
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

        static bool updateResetPassword(DBController* dbController, const resetPassword& ResetPassword)
        {
            try
            {
                nanodbc::statement stmt(dbController->statement("UPDATE reset_password SET email = ?, new_password = ? WHERE id = ?;"));
                stmt.bind(0, ResetPassword.getEmail().c_str());
                stmt.bind(1, ResetPassword.getNewPassword().c_str());
                stmt.bind(2, &ResetPassword.getID());
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

        inline std::vector<resetPassword>   getAllResetPasswordes()                                 { return getAllResetPasswordes(m_dbController); }
        inline resetPassword                getResetPasswordByID(uint32_t id)                       { return getResetPasswordByID(m_dbController, id); }
        inline bool                         createResetPassword(const resetPassword& ResetPassword) { return createResetPassword(m_dbController, ResetPassword); }
        inline bool                         updateResetPassword(const resetPassword& ResetPassword) { return updateResetPassword(m_dbController, ResetPassword); }
        inline bool                         deleteResetPassword(uint32_t id)                        { return deleteResetPassword(m_dbController, id); }
    private:
        DBController* m_dbController;
    };
}
