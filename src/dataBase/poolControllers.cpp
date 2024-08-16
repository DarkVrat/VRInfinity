#include "poolControllers.h"

std::map<DB::DBController*, std::unique_ptr<std::mutex>> DB::poolControllers::controllers;

void DB::poolControllers::init(size_t poolSize, const std::string& dbConfig)
{
    controllers.clear();

    for (size_t i = 0; i < poolSize; ++i)
        controllers.emplace(new DBController(dbConfig), std::make_unique<std::mutex>());
}

void DB::poolControllers::clear()
{
    for (auto& pair : controllers)
        delete pair.first;

    controllers.clear();
}

DB::DBController* DB::poolControllers::getController()
{
    while (true) {
        for (auto& pair : controllers) {
            if (pair.second->try_lock())
            {
                std::cout << "{!}   lock " << pair.first << " controller" << std::endl;
                return pair.first;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void DB::poolControllers::releaseController(DBController* controller)
{
    auto it = controllers.find(controller);
    if (it != controllers.end()) {
        std::cout << "{!} UNlock " << controller << " controller" << std::endl;
        it->second->unlock();
    }
}
