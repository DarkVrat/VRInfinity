#pragma once

#include <iostream>

namespace DB
{
    class imageNews
    {
    public:
        imageNews() : m_ID(0), m_image(""), m_nextID(0) {}
        imageNews(int32_t id, std::string image, int32_t nextID)
            : m_ID(id), m_image(image), m_nextID(nextID) {}

        // Get
        const int32_t& getID()          const { return m_ID;        }
        const std::string& getImage()   const { return m_image;     }
        const int32_t& getNextID()      const { return m_nextID;    }

        // Set
        void setID(int32_t id)                  { m_ID = id;            }
        void setImage(const std::string& image) { m_image = image;      }
        void setNextID(int32_t nextID)          { m_nextID = nextID;    }

        // Other
        void printVisitInfo() const
        {
            std::cout << "ID: " << m_ID << ", image: " << m_image << ", nextID: " << m_nextID  << std::endl;
        }
    private:
        int32_t m_ID;
        std::string m_image;
        int32_t m_nextID;
    };
}
