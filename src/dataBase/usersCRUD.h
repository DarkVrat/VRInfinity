#pragma once

#include "users.h"
#include "DBController.h"

namespace DB
{
	class usersCRUD
	{
	public:

        static bool createUser(DBController* dbController, const Users& user)
        {
            return true;
        }

        static Users getUserByID(DBController* dbController, uint32_t id)
        {
            Users user;
            
            return user;
        }

        static void updateUser(DBController* dbController, const Users& user)
        {
            
        }

        static void deleteUser(DBController* dbController, uint32_t id)
        {
            
        }
	};
}