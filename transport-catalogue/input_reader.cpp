#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>

/**
 * Парсит строку вида "10.123,  -30.1837" и возвращает пару координат (широта, долгота)
 */
detail::Coordinates Parse::Coordinates(std::string_view str) {
    static const double nan = std::nan("");

    auto not_space = str.find_first_not_of(' ');
    auto comma = str.find(',');

    if (comma == str.npos) {
        return { nan, nan };
    }

    auto not_space2 = str.find_first_not_of(' ', comma + 1);

    double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
    double lng = std::stod(std::string(str.substr(not_space2)));

    return { lat, lng };
}

/**
 * Удаляет пробелы в начале и конце строки
 */
std::string_view Trim(std::string_view string) {
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos) {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim) {
    std::vector<std::string_view> result;

    size_t pos = 0;
    while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
        auto delim_pos = string.find(delim, pos);
        if (delim_pos == string.npos) {
            delim_pos = string.size();
        }
        if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
            result.push_back(substr);
        }
        pos = delim_pos + 1;
    }

    return result;
}

/**
 * Парсит маршрут.
 * Для кольцевого маршрута (A>B>C>A) возвращает массив названий остановок [A,B,C,A]
 * Для некольцевого маршрута (A-B-C-D) возвращает массив названий остановок [A,B,C,D,C,B,A]
 */
std::vector<std::string_view> Parse::Route(std::string_view route) {
    if (route.find('>') != route.npos) {
        return Split(route, '>');
    }

    auto stops = Split(route, '-');
    std::vector<std::string_view> results(stops.begin(), stops.end());
    results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

    return results;
}

Parse::CommandDescription Parse::Command_Description(std::string_view line) {
    auto colon_pos = line.find(':');
    if (colon_pos == line.npos) {
        return {};
    }

    auto space_pos = line.find(' ');
    if (space_pos >= colon_pos) {
        return {};
    }

    auto not_space = line.find_first_not_of(' ', space_pos);
    if (not_space >= colon_pos) {
        return {};
    }

    return { std::string(line.substr(0, space_pos)),
            std::string(line.substr(not_space, colon_pos - not_space)),
            std::string(line.substr(colon_pos + 1)) };
}

void Parse::InputReader::Line(std::string_view line) {
    auto command_description = Command_Description(line);
    if (command_description) {
        commands_.push_back(std::move(command_description));
    }
} 

void Parse::InputReader::ApplyCommands(TransportCatalogue& catalogue) const {
    // Сначала добавляем все остановки
    for (const auto& command : commands_) {
        if (command.command == "Stop") {
            auto parts = Split(command.description, ',');
            if (parts.size() < 2) {
                continue; // Неверный формат, пропускаем
            }
            std::string coords_str = std::string(parts[0]) + "," + std::string(parts[1]);
            detail::Coordinates coords = Parse::Coordinates(coords_str);
            catalogue.PushStop(command.id, coords);
        }
    }

    // Затем добавляем расстояния между остановками
    for (const auto& command : commands_) {
        if (command.command == "Stop") {
            auto parts = Split(command.description, ',');
            if (parts.size() < 2) {
                continue; // Неверный формат, пропускаем
            }
            for (size_t i = 2; i < parts.size(); ++i) {
                std::string_view part = parts[i];
                size_t m_pos = part.find("m to ");
                if (m_pos == std::string_view::npos) {
                    continue; // Неверный формат, пропускаем
                }
                int distance = std::stoi(std::string(part.substr(0, m_pos)));
                std::string_view to_stop_name = Trim(part.substr(m_pos + 5)); // "m to " имеет длину 5

                const Stop* from_stop = catalogue.FindStop(command.id);
                const Stop* to_stop = catalogue.FindStop(to_stop_name);
                if (from_stop && to_stop) {
                    catalogue.SetDistance(from_stop, to_stop, distance);
                }
            }
        }
    }
     
    for (const auto& command : commands_) {
        if (command.command == "Bus") {
            auto route = Route(command.description);
            std::vector<Stop*> stops;
            for (const auto& stop_name : route) {
                const Stop* stop = catalogue.FindStop(stop_name);
                if (stop) {
                    stops.push_back(const_cast<Stop*>(stop));
                }
            }
            if (!stops.empty()) {
                bool is_looped = command.description.find('>') != std::string::npos;
                catalogue.PushBus(command.id, stops, is_looped);
            }
        }
    }
}
