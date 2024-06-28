#pragma once

#include <iostream>

namespace DB
{
    class visit
    {
    public:
        // Constructors
        visit() : m_ID(0), m_name(""), m_surname(""), m_email(""), m_start(""), m_end(""), m_service(""){}
        visit(uint32_t id, std::string name, std::string surname, std::string email, std::string start, std::string end, std::string service)
            : m_ID(id), m_name(""), m_surname(""), m_email(""), m_start(start), m_end(end), m_service("") {}

        // Get
        const uint32_t& getID()         const { return m_ID;        }
        const std::string& getName()    const { return m_name;      }
        const std::string& getSurname() const { return m_surname;   }
        const std::string& getEmail()   const { return m_email;     }
        const std::string& getStart()   const { return m_start;     }
        const std::string& getEnd()     const { return m_end;       }
        const std::string& getService() const { return m_service;   }

        // Set
        void setID      (uint32_t id)                   { m_ID = id;            }
        void setName    (const std::string& name)       { m_name = name;        }
        void setSurname (const std::string& surname)    { m_surname = surname;  }
        void setEmail   (const std::string& email)      { m_email = email;      }
        void setStart   (const std::string& start)      { m_start = start;      }
        void setEnd     (const std::string& end)        { m_end = end;          }
        void setService (const std::string& service)    { m_service = service;  }

    private:
        uint32_t m_ID;
        std::string m_name;
        std::string m_surname;
        std::string m_email;
        std::string m_start;
        std::string m_end;
        std::string m_service;
    };
}
