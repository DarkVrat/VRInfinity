#pragma once

#include <iostream>

namespace DB
{
    class resetPassword
    {
    public:
        resetPassword() : m_ID(0), m_email(""), m_newPassword("") {}
        resetPassword(uint32_t id, std::string email, std::string newPassword)
            : m_ID(id), m_email(email), m_newPassword(newPassword) {}

        // Get
        const uint32_t& getID()             const { return m_ID;            }
        const std::string& getEmail()       const { return m_email;         }
        const std::string& getNewPassword() const { return m_newPassword;   }

        // Set
        void setID(uint32_t id)                             { m_ID = id;                    }
        void setEmail(const std::string& email)             { m_email = email;              }
        void setNewPassword(const std::string& newPassword) { m_newPassword = newPassword;  }

    private:
        uint32_t m_ID;
        std::string m_email;
        std::string m_newPassword;
    };
}
