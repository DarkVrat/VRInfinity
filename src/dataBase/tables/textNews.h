#pragma once

#include <iostream>

namespace DB
{
    class textNews
    {
    public:
        textNews() : m_ID(0), m_text(""), m_nextID(0) {}
        textNews(uint32_t id, std::string text, uint32_t nextID)
            : m_ID(id), m_text(text), m_nextID(nextID) {}

        // Get
        const uint32_t& getID()         const { return m_ID; }
        const std::string& getText()    const { return m_text; }
        const uint32_t& getNextID()     const { return m_nextID; }

        // Set
        void setID(uint32_t id) { m_ID = id; }
        void setText(const std::string& text) { m_text = text; }
        void setNextID(uint32_t nextID) { m_nextID = nextID; }

    private:
        uint32_t m_ID;
        std::string m_text;
        uint32_t m_nextID;
    };
}
