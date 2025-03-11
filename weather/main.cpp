#include <Poco/FileStream.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <cctype>
#include <iostream>
#include <string>
#include "map.h"
#include "weather.h"
#include "map.h"

const std::string kTestApiKey = "";
const std::string kTestApiKeyMap = "";

int main() {
    try {
        City city;
        std::cin >> city.city;
        auto obj_map = CreateYandexMap(kTestApiKeyMap);
        Location loc = obj_map->YaLocal(city);
        auto obj = CreateYandexForecaster(kTestApiKey);
        auto forecast = obj->ForecastWeather(loc);
        std::cout << "Температура: " << forecast.temp << "\n"
                  << "Ощущается: " << forecast.feels_like;

    } catch (Poco::Exception& e) {
        std::cerr << "Error: " << e.displayText() << std::endl;
        return 1;
    }
}