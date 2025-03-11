#pragma once
#include <memory>
#include <string>
#include "weather.h"

struct City {
    std::string city = "Москва";
};

class Imap {
public:
    virtual ~Imap() = default;
    virtual Location YaLocal(const City& local) = 0;
};

std::unique_ptr<Imap> CreateYandexMap(
    const std::string& api_key,
    const std::string& api_endpoint = "https://geocode-maps.yandex.ru/1.x/");
