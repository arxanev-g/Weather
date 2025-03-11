#include "weather.h"
#include <Poco/Format.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/URI.h>
#include <memory>

class YaForecast : public IForecaster {
private:
    std::string key_;
    std::string api_endpoint_;

public:
    YaForecast(const std::string &key, const std::string &api_endpoint)
        : key_(key), api_endpoint_(api_endpoint) {
    }

    WeatherForecast ForecastWeather(std::optional<Location> where = std::nullopt) override {
        Poco::URI uri(api_endpoint_);
        if (where) {
            uri.addQueryParameter("lat", Poco::format("%.2f", where->lat));
            uri.addQueryParameter("lon", Poco::format("%.2f", where->lon));
        }
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
        std::string path = uri.getPathAndQuery();
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
        request.add("X-Yandex-Weather-Key", key_);
        session.sendRequest(request);

        Poco::Net::HTTPResponse respons;
        auto &body = session.receiveResponse(respons);
        if (respons.getStatus() != 200) {
            throw YandexAPIError(respons.getStatus(), "ERROR");
        }

        Poco::JSON::Parser parser;

        auto reply = parser.parse(body);
        auto fact = reply.extract<Poco::JSON::Object::Ptr>()->getObject("fact");

        WeatherForecast forecast;
        forecast.temp = fact->getValue<double>("temp");
        forecast.feels_like = fact->getValue<double>("feels_like");

        return forecast;
    }
};

std::unique_ptr<IForecaster> CreateYandexForecaster(const std::string &api_key,
                                                    const std::string &api_endpoint) {
    return std::make_unique<YaForecast>(api_key, api_endpoint);
}