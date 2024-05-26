#include "transport_catalogue.h"

void TransportCatalogue::PushBus(const std::string& name, const std::vector<Stop*>& stops, const bool is_looped) {
	buses_.push_back({ name, stops, is_looped });
	busname_to_bus_[buses_.back().name] = &buses_.back();
	for (auto& stop : stops) {
		std::string copy = name;
		stop_buses_[stop->name].emplace(&buses_.back());
	}
}

// добавление остановки в базу,
void TransportCatalogue::PushStop(const std::string& name, const detail::Coordinates& coordinates) {
	stops_.push_back({ name, coordinates });
	stopname_to_stop_[stops_.back().name] = &stops_.back();
}

// поиск маршрута по имени,
const Bus* TransportCatalogue::FindBus(const std::string_view name) const {
	if (busname_to_bus_.count(name)){
		return busname_to_bus_.at(name);
	}
	else {
		return nullptr;
	}
}

// поиск остановки по имени,
const Stop* TransportCatalogue::FindStop(const std::string_view name) const {
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
			if (FindStop(stop->name)) {
				unique_stops.insert(stop->name);
			}
		}
		return unique_stops.size();
		};
	bus_info.unique_count_stops = unique_stops_count(current_bus);

	// Длина маршрута в метрах
	for (auto iter = current_bus->stops.begin(); iter + 1 != current_bus->stops.end(); ++iter) {
		bus_info.len += detail::ComputeDistance(stopname_to_stop_.find((*iter)->name)->second->coords, stopname_to_stop_.find((*(iter + 1))->name)->second->coords);
	}

	return bus_info;
}

const std::set<Bus*> TransportCatalogue::GetBusesForStop(const std::string& name) const {
	auto it = stop_buses_.find(name);
	if (it != stop_buses_.end()) {
		return it->second;
	}
	return std::set<Bus*>();
}