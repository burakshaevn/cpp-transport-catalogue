#pragma once

#include "geo.h"
#include <deque>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>

using namespace std::string_literals;
using namespace std::string_view_literals;

// класс транспортного справочника

// Содержит информацию об остановке: название и координаты (широта и долгота)
struct Stop {
	std::string name;
	detail::Coordinates coords;
	std::set<std::string> buses;
};

struct Bus {
	std::string name;
	std::vector<std::string> stops;
	bool is_looped;
};

struct BusInfo {
	BusInfo()
		: name(""s)
		, count_stops(0)
		, unique_count_stops(0)
		, len(0.0)
	{}
	std::string name; // название маршрута. Оно совпадает с названием, переданным в запрос Bus.
	size_t count_stops; //  количество остановок в маршруте автобуса от stop1 до stop1 включительно.
	size_t unique_count_stops; // количество уникальных остановок, на которых останавливается автобус. Одинаковыми считаются остановки, имеющие одинаковые названия.
	double len; //  длина маршрута в метрах
};

class TransportCatalogue {
public:
	void PushBus(const std::string& name, const std::vector<std::string>& stops, const bool is_looped);

	void PushStop(const std::string& name, const detail::Coordinates& coordinates);

	const Bus* FindBus(const std::string& name) const;

	const Stop* FindStop(const std::string& name) const;

	const BusInfo GetBusInfo(const Bus* current_bus) const;

	const std::set<std::string>& GetBusesOnStop(const std::string& name) const;



private:
	// База данных остановок: name=название_остановки, coords={широта, долгота}
	std::deque<Stop> stops_;

	// Хранит <название_остановки <название_остановки, координаты_остановки>>
	std::unordered_map<std::string, const Stop*> stopname_to_stop_;

	// База данных автобусов: name - имя автобуса, stops - вектор его остановок
	std::deque<Bus> buses_;

	// Хранит <название_остановки <название_остановки, координаты_остановки>>
	std::unordered_map<std::string, const Bus*> busname_to_bus_;
};