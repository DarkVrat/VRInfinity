#pragma once

#include <iostream>

namespace DB
{
    class game
    {
    public:
        game() : m_ID(0), m_name(""), m_text(""), m_image("") {}
        game(uint32_t id, std::string name, std::string text, std::string image)
            : m_ID(id), m_name(""), m_text(""), m_image("") {}

        // Get
        const uint32_t& getID()         const { return m_ID; }
        const std::string& getName()    const { return m_name; }
        const std::string& getText()    const { return m_text; }
        const std::string& getImage()   const { return m_image; }

        // Set
        void setID(uint32_t id) { m_ID = id; }
        void setName(const std::string& name) { m_name = name; }
        void setText(const std::string& text) { m_text = text; }
        void setImage(const std::string& image) { m_image = image; }

    private:
        uint32_t m_ID;
        std::string m_name;
        std::string m_text;
        std::string m_image;
    };
}
