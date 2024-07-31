#pragma once

#include <iostream>

namespace DB
{
    class game_vr
    {
    public:
        game_vr() : m_id(0), m_name(""), m_text(""), m_image("") {}
        game_vr(uint32_t id, const std::string& name, const std::string& text, const std::string& image)
            : m_id(id), m_name(name), m_text(text), m_image(image) {}

        // Get
        const uint32_t& getId()         const { return m_id;    }
        const std::string& getName()    const { return m_name;  }
        const std::string& getText()    const { return m_text;  }
        const std::string& getImage()   const { return m_image; }

        // Set
        void setId(uint32_t id)                 { m_id = id;        }
        void setName(const std::string& name)   { m_name = name;    }
        void setText(const std::string& text)   { m_text = text;    }
        void setImage(const std::string& image) { m_image = image;  }

    private:
        uint32_t    m_id;
        std::string m_name;
        std::string m_text;
        std::string m_image;
    };
}
