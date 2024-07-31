#pragma once

#include <iostream>

namespace DB
{
    class image_news
    {
    public:
        image_news() : m_id(0), m_image(""), m_next_id(0) {}
        image_news(uint32_t id, const std::string& image, uint32_t next_id)
            : m_id(id), m_image(image), m_next_id(next_id) {}

        // Get
        const uint32_t& getId()         const   { return m_id;      }
        const std::string& getImage()   const   { return m_image;   }
        const uint32_t& getNextId()     const   { return m_next_id; }

        // Set
        void setId(uint32_t id)                 { m_id = id;            }
        void setImage(const std::string& image) { m_image = image;      }
        void setNextId(uint32_t next_id)        { m_next_id = next_id;  }

    private:
        uint32_t    m_id;
        std::string m_image;
        uint32_t    m_next_id;
    };
}
