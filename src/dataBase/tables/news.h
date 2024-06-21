#pragma once

#include <iostream>

namespace DB
{
    class news
    {
    public:
        news() : m_ID(0), m_time(""), m_subject(""), m_textNewsID(0), m_imageNewsID(0) {}
        news(uint32_t id, std::string time, std::string subject, uint32_t textNewsID, int32_t imageNewsID)
            : m_ID(id), m_time(time), m_subject(subject), m_textNewsID(textNewsID), m_imageNewsID(imageNewsID) {}

        // Get
        const uint32_t& getID()             const { return m_ID;            }
        const std::string& getTime()        const { return m_time;          }
        const std::string& getSubject()     const { return m_subject;       }
        const uint32_t& getTextNewsID()     const { return m_textNewsID;    }
        const int32_t& getImageNewsID()     const { return m_imageNewsID;   }

        // Set
        void setID(uint32_t id)                     { m_ID = id;                    }
        void setTime(const std::string& time)       { m_time = time;                }
        void setSubject(const std::string& subject) { m_subject = subject;          }
        void setTextNewsID(uint32_t textNewsID)     { m_textNewsID = textNewsID;    }
        void setImageNewsID(int32_t imageNewsID)    { m_imageNewsID = imageNewsID;  }

        // Other
        void printVisitInfo() const
        {
            std::cout << "ID: " << m_ID << ", time: " << m_time << ", subject: " << m_subject << ", textNewsID: " << m_textNewsID << ", imageNewsID: " << m_imageNewsID << std::endl;
        }
    private:
        uint32_t m_ID;
        std::string m_time;
        std::string m_subject;
        uint32_t m_textNewsID;
        int32_t m_imageNewsID;
    };
}
