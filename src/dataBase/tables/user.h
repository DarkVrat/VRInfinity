#pragma once

#include <iostream>

namespace DB
{
    class user
    {
    public:
        // Constructors
        user() : m_id(0), m_phone(""), m_password_hash(""), m_name(""), m_surname(""), m_role("") {}
        user(uint32_t id, const std::string& phone, const std::string& password_hash, const std::string& name, const std::string& surname, const std::string& role = "USER")
            : m_id(id), m_phone(phone), m_password_hash(password_hash), m_name(name), m_surname(surname), m_role(role) {}

        // Get
        const uint32_t& getId()                 const   { return m_id; }
        const std::string& getPhone()           const   { return m_phone; }
        const std::string& getPasswordHash()    const   { return m_password_hash; }
        const std::string& getName()            const   { return m_name; }
        const std::string& getSurname()         const   { return m_surname; }
        const std::string& getRole()            const   { return m_role; }

        // Set
        void setId(uint32_t id)                                 { m_id = id;                        }
        void setPhone(const std::string& phone)                 { m_phone = phone;                  }
        void setPasswordHash(const std::string& password_hash)  { m_password_hash = password_hash;  }
        void setName(const std::string& name)                   { m_name = name;                    }
        void setSurname(const std::string& surname)             { m_surname = surname;              }
        void setRole(const std::string& role)                   { m_role = role;                    }

    private:
        uint32_t    m_id;
        std::string m_phone;
        std::string m_password_hash;
        std::string m_name;
        std::string m_surname;
        std::string m_role;
    };
}
