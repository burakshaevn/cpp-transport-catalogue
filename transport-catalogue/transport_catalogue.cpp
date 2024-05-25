#include "transport_catalogue.h"

// добавление маршрута в базу,
void TransportCatalogue::PushBus(const std::string& name, const std::vector<std::string>& stops, const bool is_looped) {
	buses_.push_back({ name, stops, is_looped });
	for (const auto& stop : stops) {
		for (auto& stop_ : stops_) {
			if (stop == stop_.name) {
				stop_.buses.emplace(name);
			}
		}
	}
	busname_to_bus_[buses_.back().name] = &buses_.back();
}

// добавление остановки в базу,
void TransportCatalogue::PushStop(const std::string& name, const detail::Coordinates& coordinates) {
	stops_.push_back({ name, coordinates, std::set<std::string>() });
	stopname_to_stop_[stops_.back().name] = &stops_.back();
}

// поиск маршрута по имени,
const Bus* TransportCatalogue::FindBus(const std::string& name) const {
	if (busname_to_bus_.count(name)) {
		return busname_to_bus_.at(name);
	}
	else {
		return nullptr;
	}
}

// поиск остановки по имени,
const Stop* TransportCatalogue::FindStop(const std::string& name) const {
	if (stopname_to_stop_.count(name)) {
		return stopname_to_stop_.at(name);
	}
	else {
		return nullptr;
	}
}

// получение информации о маршруте в формате Bus X: R stops on route, U unique stops, L route length.
const BusInfo TransportCatalogue::GetBusInfo(const Bus* current_bus) const {
	BusInfo bus_info;

	// Подсчёт количества оставновок stop_size
	// Кольцевой: 1-2-3-1 (4)
	// Некольцевой: 1-2-3-4-3-2-1 (7)
	bus_info.count_stops = current_bus->stops.size();

	// Подсчёт уникальных остановок 
	auto unique_stops_count = [&](const Bus* current_bus) {
		std::unordered_set<std::string> unique_stops;
		for (const auto& stop : current_bus->stops) {
			if (FindStop(stop)) {
				unique_stops.insert(stop);
			}
		}
		return unique_stops.size();
		};
	bus_info.unique_count_stops = unique_stops_count(current_bus);

	// Длина маршрута в метрах
	for (auto iter = current_bus->stops.begin(); iter + 1 != current_bus->stops.end(); ++iter) {
		bus_info.len += ComputeDistance(stopname_to_stop_.find(*iter)->second->coords, stopname_to_stop_.find(*(iter + 1))->second->coords);
	}

	return bus_info;
}

const std::set<std::string>& TransportCatalogue::GetBusesOnStop(const std::string& name) const {
	return stopname_to_stop_.at(name)->buses;
}