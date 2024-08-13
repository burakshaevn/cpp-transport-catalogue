#include "transport_router.h" 

const graph::DirectedWeightedGraph<double>& TransportRouter::BuildGraph(const TransportCatalogue& catalogue) { 
    stop_name_to_vertex_id_.clear();
    graph::DirectedWeightedGraph<double> stops_graph(catalogue.GetSortedStops().size() * 2);
    std::unordered_map<std::string, graph::VertexId> stop_ids;
    graph::VertexId vertex_id = 0;

    for (const auto& [stop_name, stop_info] : catalogue.GetSortedStops()) {
        stop_ids[stop_info->name] = vertex_id;
        stops_graph.AddEdge({
                stop_info->name,
                0,
                vertex_id,
                ++vertex_id,
                static_cast<double>(bus_wait_time_)
            });
        ++vertex_id;
    }
    stop_name_to_vertex_id_ = std::move(stop_ids);

    for (const auto& bus_item : catalogue.GetSortedBuses()) {
        const Bus* bus_info = bus_item.second;
        const std::vector<const Stop*> stops = bus_info->stops;
        size_t stops_count = stops.size();

        for (size_t i = 0; i < stops_count; ++i) {
            for (size_t j = i + 1; j < stops_count; ++j) {
                const Stop* stop_from = stops[i];
                const Stop* stop_to = stops[j];
                int dist_sum = 0;
                int dist_sum_inverse = 0;

                for (size_t k = i + 1; k <= j; ++k) {
                    dist_sum += catalogue.GetDistance(stops[k - 1], stops[k]);
                    dist_sum_inverse += catalogue.GetDistance(stops[k], stops[k - 1]);
                }

                stops_graph.AddEdge({
                    bus_info->name,
                    j - i,
                    stop_name_to_vertex_id_.at(stop_from->name) + 1,
                    stop_name_to_vertex_id_.at(stop_to->name),
                    static_cast<double>(dist_sum) / (bus_velocity_ * (100.0 / 6.0))
                    });

                if (!bus_info->is_roundtrip) {
                    stops_graph.AddEdge({
                        bus_info->name,
                        j - i,
                        stop_name_to_vertex_id_.at(stop_to->name) + 1,
                        stop_name_to_vertex_id_.at(stop_from->name),
                        static_cast<double>(dist_sum_inverse) / (bus_velocity_ * (100.0 / 6.0))
                        });
                }
            }
        }
    } 

    graph_ = std::move(stops_graph);
    router_ = std::make_unique<graph::Router<double>>(graph_);

    return graph_;
}