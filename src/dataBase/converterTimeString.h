#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// Функция для преобразования строки в временную метку
std::chrono::system_clock::time_point stringToTimepoint(const std::string& timestamp) {
    std::tm tm = {};
    std::istringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp string");
    }
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

// Функция для преобразования временной метки в строку TIMESTAMP MySQL
std::string timepointToString(const std::chrono::system_clock::time_point& timepoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timepoint);
    std::tm* tm = std::localtime(&time);
    if (!tm) {
        throw std::runtime_error("Failed to convert timepoint to local time");
    }
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}