#include "bookingLogic.h"

#include "dataBase/CRUD/service_CRUD.h"
#include "dataBase/CRUD/visits_vr_CRUD.h"
#include "dataBase/CRUD/visits_smallhall_CRUD.h"
#include "localParser.h"

// Получение доступных временных меток
std::string bookingLogic::getAvailableTime(const std::string& inputDate, DB::DBController* dbController, uint32_t service_id)
{
    // Получение формулы и области поиска
    std::string Formula = DB::service_CRUD::getServiceByID(dbController, service_id).getFormula();
    std::string MinTime = getMinTime(inputDate);
    std::string MaxTime = getMaxTime(inputDate, Formula);

    // Получение всех записей между MinTime и MaxTime
    std::vector<DB::visits_vr> VR_visits = DB::visits_vr_CRUD::getVisitsVrInRange(dbController, MinTime, MaxTime);
    std::vector<DB::visits_smallhall> smallhall_visits = DB::visits_smallhall_CRUD::getVisitsSmallhallInRange(dbController, MinTime, MaxTime);

    // Создание и заполнение массива из получасовых меток для поиска окна 
    std::tm minTM = stringToTm(MinTime);
    std::tm maxTM = stringToTm(MaxTime);

    int difference = calculateHalfHourIntervals(minTM, maxTM) + 1;

    std::vector<SlotAvailability> slots(difference);

    int buffer = -1;
    int left = -1;
    int right = -1;
    for (int i = 0; i < VR_visits.size(); ++i)
    {
        if (buffer == -1 || VR_visits[i].getVisitId() != VR_visits[buffer].getVisitId())
        {
            auto& curent = VR_visits[i];
            left = calculateHalfHourIntervals(minTM, stringToTm(curent.getStart()));
            right = calculateHalfHourIntervals(minTM, stringToTm(curent.getEnd()));
            buffer = i;
        }

        for (int j = left; j <= right; ++j)
            ++slots[j].vrPoints;
    }

    for (int i = 0; i < smallhall_visits.size(); ++i)
    {
        auto& curent = smallhall_visits[i];
        left = calculateHalfHourIntervals(minTM, stringToTm(curent.getStart()));
        right = calculateHalfHourIntervals(minTM, stringToTm(curent.getEnd()));

        for (int j = left; j <= right; ++j)
            ++slots[j].smallHall;
    }

    // Получение поиск точек подходящих под услугу
    int VR_need = localParser::parseFormula(Formula, localParser::FORMULA::VR);
    int SH_need = localParser::parseFormula(Formula, localParser::FORMULA::SMALLHALL);
    int range = localParser::parseFormula(Formula, localParser::FORMULA::HOURS) * 2 + (localParser::parseFormula(Formula, localParser::FORMULA::MINUTES) + 29) / 30;
    std::vector<int> points;
    points.reserve(difference - range);

    int leftIT = 0;
    for (int rightIT = 0; rightIT < slots.size(); ++rightIT)
    {
        if (slots[rightIT].vrPoints + VR_need > 4 || slots[rightIT].smallHall + SH_need > 1)
            leftIT = rightIT + 1;
        else if (rightIT - leftIT >= range)
        {
            points.push_back(leftIT);
            ++leftIT;
        }
    }

    // Заполнение ответа
    int iter = 0;
    std::tm currentTM = minTM;
    std::ostringstream oss;
    std::string response_data = "{\"times\":[";
    for (int i = 0; i < difference - range; ++i)
    {
        if (points[iter] == i)
        {
            oss.str("");
            oss << std::put_time(&currentTM, "%H:%M");
            response_data += "\"" + oss.str() + "\"";
            ++iter;
            if (iter < points.size())
                response_data += ",";
        }
        else if (points[iter] < i)
        {
            while (iter < points.size() && points[iter] <= i) {
                ++iter;
            }
        }
        currentTM.tm_min += 30;
        std::mktime(&currentTM);
    }
    response_data += "]}";

    return response_data;
}

// Вычесление получасовых интервалов между 2 метками
int bookingLogic::calculateHalfHourIntervals(const std::tm& start, const std::tm& end)
{
    std::time_t startTime = std::mktime(const_cast<std::tm*>(&start));
    std::time_t endTime = std::mktime(const_cast<std::tm*>(&end));
    return static_cast<int>(std::difftime(endTime, startTime) / (30 * 60));
}

// Получение минивального времени отсчёта
std::string bookingLogic::getMinTime(const std::string& inputDate)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream ossToday;
    ossToday << std::put_time(&now_tm, "%Y-%m-%d");

    if (inputDate == ossToday.str()) {
        now_tm.tm_hour += 1;
        now_tm.tm_sec = 0;
        ceilTime(now_tm);
    }
    else
        return inputDate + " 11:00:00";

    std::ostringstream ossMinTime;
    ossMinTime << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return ossMinTime.str();
}

// Получение максимального времени отсчёта
std::string bookingLogic::getMaxTime(const std::string& inputDate, const std::string& serviceFormula)
{
    int hours = localParser::parseFormula(serviceFormula, localParser::FORMULA::HOURS);
    int vrPoints = localParser::parseFormula(serviceFormula, localParser::FORMULA::VR);

    int maxHour = 21;
    if (vrPoints >= 3 && hours >= 2)
        maxHour = 1;

    std::tm inputTm = stringToTm(inputDate);

    if (maxHour == 1)
        inputTm.tm_mday += 1;
    inputTm.tm_hour = maxHour;
    inputTm.tm_min = 0;
    inputTm.tm_sec = 0;

    std::ostringstream oss;
    oss << std::put_time(&inputTm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Парсинг string в std::tm
std::tm bookingLogic::stringToTm(const std::string& timeStr)
{
    std::tm tm = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return tm;
}

// Округление std::tm к получасовым меткам
void bookingLogic::ceilTime(std::tm& tm)
{
    if (tm.tm_min > 0 && tm.tm_min < 30)
        tm.tm_min = 30;
    else if (tm.tm_min > 30)
    {
        ++tm.tm_hour;
        tm.tm_min = 0;
    }

    std::mktime(&tm);
}
