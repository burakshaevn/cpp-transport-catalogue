#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

// обработка запросов на заполнение справочника
namespace Parse {

    detail::Coordinates Coordinates(std::string_view str);

    std::vector<std::string_view> Route(std::string_view route); 

    struct CommandDescription {
        // Определяет, задана ли команда (поле command непустое)
        explicit operator bool() const {
            return !command.empty();
        }

        bool operator!() const {
            return !operator bool();
        }

        std::string command;      // Название команды
        std::string id;           // id маршрута или остановки
        std::string description;  // Параметры команды
    };

    CommandDescription Command_Description(std::string_view line);

    class InputReader {
    public:
        /**
         * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
         */
        void Line(std::string_view line);

        /**
         * Наполняет данными транспортный справочник, используя команды из commands_
         */
        void ApplyCommands(TransportCatalogue& catalogue) const;

            std::pair<std::string, detail::Coordinates> FillStop(std::string& line);

        void FillStopDistances(std::string& line, TransportCatalogue& catalogue);

    private:
        std::vector<CommandDescription> commands_;
    };
}