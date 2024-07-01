#pragma once

#include <string>
#include <vector> 
#include "geo.h"

struct Stop {
    std::string name;
    detail::Coordinates coords;
};

struct Bus {
    std::string name;
    std::vector<const Stop*> stops;
    bool is_roundtrip;
};

struct BusInfo {
    std::string name;
    size_t count_stops;
    size_t unique_count_stops;
    double len;
    double curvature;
};
