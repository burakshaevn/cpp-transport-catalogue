#pragma once

#include <iosfwd>
#include <iostream>
#include <string>
#include <string_view>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "transport_catalogue.h"

// обработка запросов к базе и вывод данных

namespace Parse {
    void PrintStat(const TransportCatalogue& tansport_catalogue, std::string_view request,
        std::ostream& output);
}