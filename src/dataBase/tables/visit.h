#pragma once

#include <iostream>

namespace DB
{
    class visit
    {
    public:
        // Constructors
        visit() : m_id(0), m_name(""), m_surname(""), m_phone(""), m_start(""), m_service_id(0){}
        visit(uint32_t id, const std::string& name, const std::string& surname, const std::string& phone, const std::string& start, uint32_t service_id)
            : m_id(id), m_name(name), m_surname(surname), m_phone(phone), m_start(start), m_service_id(service_id) {}

        // Get
        const uint32_t& getId()         const   { return m_id;          }
        const std::string& getName()    const   { return m_name;        }
        const std::string& getSurname() const   { return m_surname;     }
        const std::string& getPhone()   const   { return m_phone;       }
        const std::string& getStart()   const   { return m_start;       }
        const uint32_t& getServiceId()  const   { return m_service_id;  }

        // Set
        void setId(uint32_t id)                     { m_id = id;                }
        void setName(const std::string& name)       { m_name = name;            }
        void setSurname(const std::string& surname) { m_surname = surname;      }
        void setPhone(const std::string& phone)     { m_phone = phone;          }
        void setStart(const std::string& start)     { m_start = start;          }
        void setServiceId(uint32_t service_id)      { m_service_id = service_id;}

    private:
        uint32_t    m_id;
        std::string m_name;
        std::string m_surname;
        std::string m_phone;
        std::string m_start;
        uint32_t    m_service_id;
    };
}
