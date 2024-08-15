#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "dataBase/DBController.h"

class bookingLogic
{
public:
    // ��������� ��������� ��������� �����
    static std::string getAvailableTime(const std::string& inputDate, DB::DBController* dbController, uint32_t service_id);
private:
    //��������������� �������
    struct SlotAvailability {
        uint8_t vrPoints = 0;
        uint8_t smallHall = 0;
    };

    static int calculateHalfHourIntervals(const std::tm& start, const std::tm& end);

    static std::string getMinTime(const std::string& inputDate);
    static std::string getMaxTime(const std::string& inputDate, const std::string& serviceFormula);

    static std::tm stringToTm(const std::string& timeStr);
    static void ceilTime(std::tm& tm);
};
