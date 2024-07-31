#pragma once

#include <iostream>

namespace DB
{
    class service
    {
    public:
        service() : m_id(0), m_name(""), m_formula("") {}
        service(uint32_t id, const std::string& name, const std::string& formula)
            : m_id(id), m_name(name), m_formula(formula) {}

        // Get
        const uint32_t& getId()         const   { return m_id;      }
        const std::string& getName()    const   { return m_name;    }
        const std::string& getFormula() const   { return m_formula; }

        // Set
        void setId(uint32_t id)                     { m_id = id;            }
        void setName(const std::string& name)       { m_name = name;        }
        void setFormula(const std::string& formula) { m_formula = formula;  }

    private:
        uint32_t    m_id;
        std::string m_name;
        std::string m_formula;
    };
}
