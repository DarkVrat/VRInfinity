#pragma once

#include <iostream>

namespace DB
{
    class news
    {
    public:
        news() : m_id(0), m_time(""), m_subject(""), m_text_news_id(0), m_image_news_id(0) {}
        news(uint32_t id, const std::string& time, const std::string& subject, uint32_t text_news_id, uint32_t image_news_id)
            : m_id(id), m_time(time), m_subject(subject), m_text_news_id(text_news_id), m_image_news_id(image_news_id) {}

        // Get
        const uint32_t& getId()             const { return m_id;            }
        const std::string& getTime()        const { return m_time;          }
        const std::string& getSubject()     const { return m_subject;       }
        const uint32_t& getTextNewsId()     const { return m_text_news_id;  }
        const uint32_t& getImageNewsId()    const { return m_image_news_id; }

        // Set
        void setId(uint32_t id)                     { m_id = id;                        }
        void setTime(const std::string& time)       { m_time = time;                    }
        void setSubject(const std::string& subject) { m_subject = subject;              }
        void setTextNewsId(uint32_t text_news_id)   { m_text_news_id = text_news_id;    }
        void setImageNewsId(uint32_t image_news_id) { m_image_news_id = image_news_id;  }

    private:
        uint32_t    m_id;
        std::string m_time;
        std::string m_subject;
        uint32_t    m_text_news_id;
        uint32_t    m_image_news_id;
    };

}
