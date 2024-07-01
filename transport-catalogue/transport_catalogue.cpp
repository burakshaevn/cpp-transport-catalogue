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

double GeographicalDistance(const Bus* bus) {
	double total_distance = 0.0;
	for (auto iter = bus->stops.begin(); iter + 1 != bus->stops.end(); ++iter) {
		total_distance += detail::ComputeDistance((*iter)->coords, (*(iter + 1))->coords);
	}
	return total_distance;
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
		/*if (current_bus->is_looped) {
			route_length += GetDistance(from, to);
			geographic_length += detail::ComputeDistance(from->coords,
				to->coords);
		}
		else {*/
		route_length += GetDistance(from, to);
		geographic_length += detail::ComputeDistance(from->coords, to->coords);
		//}
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
	return 0; // Или другое значение по умолчанию, если расстояние не найдено
}
