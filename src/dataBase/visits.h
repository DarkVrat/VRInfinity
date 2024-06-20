#pragma once

#include <iostream>

namespace DB
{
    class Visits
    {
    public:
        // Constructors
        Visits() : m_ID(0), m_start(""), m_end(""), m_ID_users(0){}
        Visits(uint32_t id, std::string start, std::string end, uint32_t ID_users)
            : m_ID(id), m_start(start), m_end(end), m_ID_users(ID_users) {}

        // Get
        const uint32_t& getID()         const { return m_ID;        }
        const std::string& getStart()   const { return m_start;     }
        const std::string& getEnd()     const { return m_end;       }
        const uint32_t& getIDUsers()    const { return m_ID_users;  }

        // Set
        void setID      (uint32_t id)               { m_ID = id;                }
        void setStart   (const std::string& start)  { m_start = start;          }
        void setEnd     (const std::string& end)    { m_end = end;              }
        void setIDUsers (uint32_t ID_users)         { m_ID_users = ID_users;    }

        // Other
        void printUserInfo() const
        {
            std::cout << "ID: " << m_ID << ", start: " << m_start << ", End: " << m_end << ", Id User: " << m_ID_users << std::endl;
        }
    private:
        uint32_t m_ID;
        std::string m_start;
        std::string m_end;
        uint32_t m_ID_users;
    };
}