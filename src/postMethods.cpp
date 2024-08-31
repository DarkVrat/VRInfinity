#include "postMethods.h"

#include "localParser.h"
#include "security.h"
#include "utility.h"
#include "genericHTML.h"
#include "bookingLogic.h"
#include "dataBase/CRUD/visits_vr_CRUD.h"
#include "dataBase/CRUD/visits_smallhall_CRUD.h"
#include "dataBase/poolControllers.h"

using namespace DB;

void postMethods::postBooking(const crow::request& req, crow::response& res)
{
    std::string body = req.body;
    std::string name = localParser::urlDecode(localParser::getValue(body, "name"));
    std::string surname = localParser::urlDecode(localParser::getValue(body, "surname"));
    std::string phone = localParser::urlDecode(localParser::getValue(body, "phone"));
    uint32_t service = std::atoi(localParser::getValue(body, "service").c_str());
    std::string data = localParser::getValue(body, "datepicker");
    std::string time = localParser::urlDecode(localParser::getValue(body, "time"));

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return;
    }

    std::string formula = service_CRUD::getServiceByID(dbController, service).getFormula();

    int Hours = localParser::parseFormula(formula, localParser::FORMULA::HOURS);
    int Minutes = localParser::parseFormula(formula, localParser::FORMULA::MINUTES);
    int VR = localParser::parseFormula(formula, localParser::FORMULA::VR);
    int SmallHall = localParser::parseFormula(formula, localParser::FORMULA::SMALLHALL);

    std::string start = data + " " + time + ":00";
    std::string end;

    std::tm tm = {};
    std::istringstream ss(start);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    tm.tm_hour += Hours;
    tm.tm_min += Minutes;
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    end = oss.str();

    res.redirect("/");

    dbController->startTransaction();
    uint32_t id = visit_CRUD::createVisit(dbController, visit(0, name, surname, phone, start, service));
    if (id == 0)
    {
        dbController->rollbackTransaction();
        DB::poolControllers::releaseController(dbController);
        res.end();
        return;
    }

    for (int i = 0; i < VR; ++i)
    {
        uint32_t id_vr = visits_vr_CRUD::createVisitsVr(dbController, visits_vr(0, start, end, id));
        if (id_vr == 0)
        {
            dbController->rollbackTransaction();
            DB::poolControllers::releaseController(dbController);
            res.end();
            return;
        }
    }

    if (SmallHall > 0)
    {
        uint32_t id_sh = visits_smallhall_CRUD::createVisitsSmallhall(dbController, visits_smallhall(0, start, end, id));
        if (id_sh == 0)
        {
            dbController->rollbackTransaction();
            DB::poolControllers::releaseController(dbController);
            res.end();
            return;
        }
    }


    dbController->commitTransaction();
    DB::poolControllers::releaseController(dbController);
    res.end();
}

crow::response postMethods::postGetAvailableTimes(const crow::request& req)
{
    auto request_data = crow::json::load(req.body);
    if (!request_data) {
        return crow::response(400, "Invalid JSON");
    }

    uint32_t serviceId = request_data["serviceId"].i();
    std::string date = request_data["date"].s();

    DBController* dbController = DB::poolControllers::getController();
    if (!dbController)
    {
        crow::response res;
        res.code = 500;
        res.write("Internal Server Error: Could not obtain DB controller.");
        res.end();
        return res;
    }

    std::string TIMES = bookingLogic::getAvailableTime(date, dbController, serviceId);
    DB::poolControllers::releaseController(dbController);

    crow::response res{ TIMES };
    res.add_header("Content-Type", "application/json");
    return res;
}
