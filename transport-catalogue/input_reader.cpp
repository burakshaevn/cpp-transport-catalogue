#include "stat_reader.h" 

void Parse::PrintStat(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output) {
    if (request.starts_with("Bus")) {
        std::string bus_name(request.substr(4)); // Пропускаем "Bus " в начале
        const Bus* current_bus = transport_catalogue.FindBus(bus_name);

        if (!current_bus) {
            output << "Bus " << bus_name << ": not found"s << std::endl;
        }
        else {
            BusInfo bus_info = transport_catalogue.GetBusInfo(current_bus);
            output << "Bus " << bus_name << ": "
                << bus_info.count_stops << " stops on route, "
                << bus_info.unique_count_stops << " unique stops, "
                << std::setprecision(6) << bus_info.len << " route length\n";
        }
    }
    else if (request.starts_with("Stop")) {
        /* Запрос должен вывести информацию об остановке X в следующем формате:
        bus1 bus2 ... busN — список маршрутов, проходящих через остановку.
        Дубли не допускаются, названия должны быть отсортированы в алфавитном порядке. */
        std::string stop_name(request.substr(5));
        auto current_stop = transport_catalogue.FindStop(stop_name);

        // Если остановка X не найдена, выведите: "Stop X: not found".
        if (!current_stop) {
            output << "Stop "s << stop_name << ": not found"s << std::endl;
        }
        else { // Остановка существует  
            std::set<std::string> buses = transport_catalogue.GetBusesOnStop(stop_name);
            if (buses.empty()) { // Если остановка X существует в базе, но через неё не проходит ни один маршрут, выведите: "Stop X: no buses".
                output << "Stop "s << stop_name << ": no buses"s << std::endl;
            }
            else { // Остановка существует в базе и по ней проходит минимум 1 маршрут, будет выведено "Stop X: buses bus1 bus2 ... busN".
                output << "Stop "s << stop_name << ": buses "s;
                for (const auto& bus : buses) {
                    output << bus << " "s;
                }
                output << std::endl;
            }
        }
    }

    else {
        // Обработка других типов запросов 
    }
}