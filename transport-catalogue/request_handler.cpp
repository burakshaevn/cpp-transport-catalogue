// Код обработчика запросов к базе, содержащего логику 

#include "request_handler.h"
#include <algorithm> 

std::optional<BusInfo> RequestHandler::GetBusStat(const std::string_view& bus_number) const {
    BusInfo bus_stat{};
    const Bus* bus = catalogue_.FindBus(bus_number);

    if (!bus) throw std::invalid_argument("bus not found");

    bus_stat.count_stops = bus->is_roundtrip ? bus->stops.size() : bus->stops.size() * 2 - 1;

    int route_length = 0;
    double geographic_length = 0.0;

    for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
        auto from = bus->stops[i];
        auto to = bus->stops[i + 1];

        route_length += catalogue_.GetDistance(from, to);
        geographic_length += detail::ComputeDistance(from->coords, to->coords);

        if (!bus->is_roundtrip) {
            route_length += catalogue_.GetDistance(to, from);
            geographic_length += detail::ComputeDistance(to->coords, from->coords);
        }
    }

    bus_stat.unique_count_stops = catalogue_.ComputeUniqueStopsCount(bus_number);
    bus_stat.len = route_length;
    bus_stat.curvature = route_length / geographic_length;

    return bus_stat;
} 

svg::Document RequestHandler::RenderMap() const {
    return renderer_.GetSVG(catalogue_.GetSortedBuses());
}

const TransportCatalogue& RequestHandler::GetCatalogue() const {
    return catalogue_;
}

const RequestHandler::Graph& RequestHandler::GetTransportRouterGraph() const {
    return transport_router_.GetGraph();
}

const std::optional<graph::Router<double>::RouteInfo> RequestHandler::GetOptimalRoute(const std::string_view from, const std::string_view to) const {
    try {
        return transport_router_.GetRouter().get()->BuildRoute(transport_router_.GetVertexId(from), transport_router_.GetVertexId(to));
    }
    catch (const std::invalid_argument&) {
        return std::nullopt;
    }
}