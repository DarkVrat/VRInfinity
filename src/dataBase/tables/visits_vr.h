#pragma once

#include <iostream>

namespace DB
{
    class visits_vr
    {
    public:
        visits_vr() : m_id(0), m_start(""), m_end(""), m_visit_id(0) {}
        visits_vr(uint32_t id, const std::string& start, const std::string& end, uint32_t visit_id)
            : m_id(id), m_start(start), m_end(end), m_visit_id(visit_id) {}

        // Get
        const uint32_t& getId()         const   { return m_id;      }
        const std::string& getStart()   const   { return m_start;   }
        const std::string& getEnd()     const   { return m_end;     }
        const uint32_t& getVisitId()    const   { return m_visit_id;}

        // Set
        void setId(uint32_t id)                 { m_id = id;            }
        void setStart(const std::string& start) { m_start = start;      }
        void setEnd(const std::string& end)     { m_end = end;          }
        void setVisitId(uint32_t visit_id)      { m_visit_id = visit_id;}

    private:
        uint32_t    m_id;
        std::string m_start;
        std::string m_end;
        uint32_t    m_visit_id;
    };
}
