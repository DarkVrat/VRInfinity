#pragma once

#include <iostream>

namespace DB
{
	class user
	{
	public:
        // Constructors
        user() : m_ID(0), m_email(""), m_passwordHash(""), m_name(""), m_surname(""), m_role("") {}
        user(uint32_t id, std::string email, std::string passwordHash, std::string name, std::string surname, std::string role = "USER")
            : m_ID(id), m_email(email), m_passwordHash(passwordHash), m_name(name), m_surname(surname), m_role(role) {}

        // Get
        const uint32_t&    getID()             const { return m_ID;            }
        const std::string& getEmail()          const { return m_email;         }
        const std::string& getPasswordHash()   const { return m_passwordHash;  }
        const std::string& getName()           const { return m_name;          }
        const std::string& getSurname()        const { return m_surname;       }
        const std::string& getRole()           const { return m_role;          }

        // Set
        void setID          (uint32_t id)                       { m_ID = id;                        }
        void setEmail       (const std::string& email)          { m_email = email;                  }
        void setPasswordHash(const std::string& passwordHash)   { m_passwordHash = passwordHash;    }
        void setName        (const std::string& name)           { m_name = name;                    }
        void setSurname     (const std::string& surname)        { m_surname = surname;              }
        void setRole        (const std::string& role)           { m_role = role;                    }

        // Other
        void printUserInfo() const 
        {
            std::cout   << "ID: " << m_ID << ", Email: " << m_email << ", Name: " << m_name << ", Surname: " << m_surname << ", Role: " << m_role << std::endl;
        }

	private:
		uint32_t m_ID;
		std::string m_email;
		std::string m_passwordHash;
		std::string m_name;
		std::string m_surname;
		std::string m_role;
	};
}
