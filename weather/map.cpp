#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/URI.h>
#include <string>
#include "weather.h"
#include "map.h"

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    std::vector<std::string> res;
    if (string.empty()) {
        return res;
    }
    size_t current_pos = 0;
    size_t prev_pos = 0;
    while ((current_pos = string.find(delimiter, current_pos)) != std::string::npos) {
        res.push_back(string.substr(prev_pos, current_pos - prev_pos));
        current_pos += delimiter.length();
        prev_pos = current_pos;
    }
    res.push_back(string.substr(prev_pos, string.length() - prev_pos));

    return res;
}

class YaMap : public Imap {
private:
    std::string key_;
    std::string api_endpoint_;

public:
    YaMap(const std::string& key, const std::string& api_endpoint)
        : key_(key), api_endpoint_(api_endpoint) {
    }

    Location YaLocal(const City& local) override {
        Poco::URI uri(api_endpoint_);
        uri.addQueryParameter("apikey", key_);
        uri.addQueryParameter("geocode", local.city);
        uri.addQueryParameter("format", "json");
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
        std::string path = uri.getPathAndQuery();
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
        session.sendRequest(request);

        Poco::Net::HTTPResponse respons;
        auto& body = session.receiveResponse(respons);
        if (respons.getStatus() != 200) {
            throw YandexAPIError(respons.getStatus(), "ERROR");
        }

        Poco::JSON::Parser parser;

        auto reply = parser.parse(body);
        auto fact = reply.extract<Poco::JSON::Object::Ptr>()
                        ->getObject("response")
                        ->getObject("GeoObjectCollection")
                        ->getArray("featureMember")
                        ->getObject(0)
                        ->getObject("GeoObject")
                        ->getObject("Point")
                        ->getValue<std::string>("pos");
        auto vec_cord = Split(fact);

        Location forecast;
        forecast.lat = std::stod(vec_cord[0]);
        forecast.lon = std::stod(vec_cord[1]);

        return forecast;
    }
};

std::unique_ptr<Imap> CreateYandexMap(const std::string& api_key, const std::string& api_endpoint) {
    return std::make_unique<YaMap>(api_key, api_endpoint);
}