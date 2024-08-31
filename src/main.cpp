#include "crow.h"

#include "utility.h"
#include "getMethods.h"
#include "postMethods.h"
#include "dataBase/poolControllers.h"

int main(int argc, char** argv)
{
    utility::setExecutablePath(argv[0]);

    DB::DBController* conn = new DB::DBController(utility::readFile("res/DBConfig.txt"));
    auto stmt = conn->statement("SHOW VARIABLES LIKE 'max_connections';");
    auto result = nanodbc::execute(stmt);

    if (result.next()) 
    {
        std::string name = result.get<std::string>(0);
        int max_connections = result.get<int>(1);
        DB::poolControllers::init(max_connections, utility::readFile("res/DBConfig.txt"));
        std::cout << "{!} Init " << max_connections << " connetors" << std::endl;
    }
    else
        std::cerr << "{E} Failed init connetors" << std::endl;

    delete conn;

    crow::SimpleApp app;

    // -=-=- Потом удалить, и настроить через nginx -=-=-
    CROW_ROUTE(app, "/css/<string>")
        ([](const std::string& filename) {
        std::string content = utility::readFile("res/css/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "text/css");
        res.write(content);
        return res;
    });

    CROW_ROUTE(app, "/js/<string>")
        ([](const std::string& filename) {
        std::string content = utility::readFile("res/js/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "application/javascript");
        res.write(content);
        return res;
    });

    CROW_ROUTE(app, "/image/<string>")
        ([](const std::string& filename) {
        std::vector<char> content = utility::readFileImage("res/image/" + filename);

        if (content.empty()) {
            return crow::response(404);
        }

        crow::response res;
        res.set_header("Content-Type", "image/jpeg");
        res.body.assign(content.begin(), content.end());
        return res;
    });
    // -=-=- Потом удалить, и настроить через nginx -=-=-

    // -=-=- Get Методы  -=-=- 
    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)
        ([](const crow::request& req, crow::response& res) { 
        getMethods::getIndex(req, res); 
    });

    CROW_ROUTE(app, "/news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getNews(req, res);
    });

    CROW_ROUTE(app, "/full_news").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getFullNews(req, res);
    });

    CROW_ROUTE(app, "/catalog").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getCatalog(req, res);
    });

    CROW_ROUTE(app, "/booking").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getBooking(req, res);
    });

    CROW_ROUTE(app, "/adminPanel").methods("GET"_method)
        ([](const crow::request& req, crow::response& res) {
        getMethods::getAdminPanel(req, res);
    });
    // -=-=- GET Методы  -=-=- 

    // -=-=- POST Методы -=-=-
    CROW_ROUTE(app, "/get-available-times").methods(crow::HTTPMethod::POST)
        ([](const crow::request& req) {
        return postMethods::postGetAvailableTimes(req);
    });

    CROW_ROUTE(app, "/booking").methods("POST"_method)
        ([](const crow::request& req, crow::response& res) {
        postMethods::postBooking(req, res);
    });
    // -=-=- POST Методы -=-=-

    app.port(18080).multithreaded().run();
}
