#pragma once

#include <iostream>

namespace DB
{
    class reset_password
    {
    public:
        reset_password() : m_id(0), m_phone(""), m_new_password("") {}
        reset_password(uint32_t id, const std::string& phone, const std::string& new_password)
            : m_id(id), m_phone(phone), m_new_password(new_password) {}

        // Get
        const uint32_t& getId()             const { return m_id;            }
        const std::string& getPhone()       const { return m_phone;         }
        const std::string& getNewPassword() const { return m_new_password;  }

        // Set
        void setId(uint32_t id)                                 { m_id = id;                    }
        void setPhone(const std::string& phone)                 { m_phone = phone;              }
        void setNewPassword(const std::string& new_password)    { m_new_password = new_password;}

    private:
        uint32_t    m_id;
        std::string m_phone;
        std::string m_new_password;
    };
}
