#pragma once

#include <iostream>

namespace DB
{
    class text_news
    {
    public:
        text_news() : m_id(0), m_text(""), m_next_id(0) {}
        text_news(uint32_t id, const std::string& text, uint32_t next_id)
            : m_id(id), m_text(text), m_next_id(next_id) {}

        // Get
        const uint32_t& getId()         const   { return m_id;      }
        const std::string& getText()    const   { return m_text;    }
        const uint32_t& getNextId()     const   { return m_next_id; }

        // Set
        void setId(uint32_t id)                 { m_id = id;            }
        void setText(const std::string& text)   { m_text = text;        }
        void setNextId(uint32_t next_id)        { m_next_id = next_id;  }

    private:
        uint32_t    m_id;
        std::string m_text;
        uint32_t    m_next_id;
    };
}
