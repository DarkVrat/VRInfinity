#pragma once

#include "DBController.h"

#include <map>
#include <mutex>
#include <thread>
#include <chrono>

namespace DB
{
    class poolControllers {
    public:
        // �������� � �������� �����������
        static void init(size_t poolSize, const std::string& dbConfigPath);
        static void clear();

        // ��������� � ������������ ������������
        static DBController* getController();
        static void releaseController(DBController* controller);
    private:
        static std::map<DBController*, std::unique_ptr<std::mutex>> controllers;
    };
}
