#include "transport_catalogue.h" 

// добавление маршрута в базу,
void TransportCatalogue::AddBus(const std::string_view name, std::vector<const Stop*>& stops, const bool is_roundtrip) {
	buses_.push_back({ std::string(name), stops, is_roundtrip });
	busname_to_bus_[buses_.back().name] = &buses_.back();
	for (auto& stop : stops) { 
		stop_buses_[stop->name].emplace(&buses_.back());
	} 
}

// добавление остановки в базу,
void TransportCatalogue::AddStop(const std::string_view name, const detail::Coordinates& coordinates) {
	stops_.push_back({ std::string(name), coordinates });
	stopname_to_stop_[stops_.back().name] = &stops_.back();
} 

// поиск маршрута по имени,
const Bus* TransportCatalogue::FindBus(const std::string_view name) const {
	auto iter = busname_to_bus_.find(name);
	if (iter != busname_to_bus_.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

// поиск остановки по имени,
const Stop* TransportCatalogue::FindStop(const std::string_view name) const {
	auto iter = stopname_to_stop_.find(name);
	if (iter != stopname_to_stop_.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
} 

const BusInfo TransportCatalogue::GetBusInfo(const Bus* current_bus) const {
	BusInfo bus_info;

	// Подсчёт количества оставновок stop_size
	bus_info.count_stops = current_bus->stops.size();

	// Подсчёт уникальных остановок 
	std::unordered_set<std::string> unique_stops;
	for (const auto& stop : current_bus->stops) {
		unique_stops.insert(stop->name);
	}
	bus_info.unique_count_stops = unique_stops.size();

	// Длина маршрута в метрах
	int route_length = 0;
	double geographic_length = 0.0;

	for (size_t i = 0; i < current_bus->stops.size() - 1; ++i) {
		auto from = current_bus->stops[i];
		auto to = current_bus->stops[i + 1]; 
		route_length += GetDistance(from, to);
		geographic_length += detail::ComputeDistance(from->coords, to->coords); 
	}
	 
	bus_info.len = route_length;
	bus_info.curvature = route_length / geographic_length;
	return bus_info;
}

const std::set<Bus*> TransportCatalogue::GetBusesForStop(const std::string_view name) const {
	auto it = stop_buses_.find(name);
	if (it != stop_buses_.end()) {
		return it->second;
	}
	return std::set<Bus*>();
}

void TransportCatalogue::SetDistance(const Stop* from, const Stop* to, int distance) {
	distances_[{from, to}] = distance;
} 

int TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const {
	if (auto it = distances_.find({ from, to }); it != distances_.end()) {
		return it->second;
	}
	// Проверка на обратное направление, если расстояние не найдено в прямом направлении
	if (auto it = distances_.find({ to, from }); it != distances_.end()) {
		return it->second;
	}
	return 0;  
}

size_t TransportCatalogue::ComputeUniqueStopsCount(std::string_view bus_number) const {
	std::unordered_set<std::string_view> unique_stops;
	for (const auto& stop : busname_to_bus_.at(bus_number)->stops) {
		unique_stops.insert(stop->name);
	}
	return unique_stops.size();
}

const std::map<std::string_view, const Bus*> TransportCatalogue::GetSortedBuses() const {
	std::map<std::string_view, const Bus*> result;
	for (const auto& bus : busname_to_bus_) {
		result.emplace(bus);
	}
	return result;
}

const std::map<std::string_view, const Stop*> TransportCatalogue::GetSortedStops() const {
	std::map<std::string_view, const Stop*> result;
	for (const auto& stop : stopname_to_stop_) {
		result.emplace(stop);
	}
	return result;
}