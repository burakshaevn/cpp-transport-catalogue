#pragma once
#include "json.h"
#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"
#include <utility>
#include <memory>

class TransportRouter {
public:

	using Graph = graph::DirectedWeightedGraph<double>;
	using Router = std::unique_ptr<graph::Router<double>>;

	TransportRouter() = default;

	template <typename WaitTime, typename Velocity>
	explicit TransportRouter(WaitTime&& wait_time, Velocity&& velocity, const TransportCatalogue& catalogue) {
		bus_wait_time_ = std::forward<WaitTime>(wait_time);
		bus_velocity_ = std::forward<Velocity>(velocity);
		catalogue_ = catalogue;
		BuildGraph();
	}

	int GetBusWaitTime() const {
		return bus_wait_time_;
	}
	
	double GetBusVelocity() const {
		return bus_velocity_;
	}

	const Graph& GetGraph() const {
		return graph_;
	}
	
	const Router& GetRouter() const {
		return router_;
	}
	
	Router& GetRouter() {
		return router_;
	}

	graph::VertexId GetVertexId(const std::string_view stop_name) const {
		auto it = stop_name_to_vertex_id_.find(std::string(stop_name));
		if (it != stop_name_to_vertex_id_.end()) {
			return it->second;
		}
		throw std::invalid_argument("Stop name not found");
	}

	const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;

	const Graph& BuildGraph();
	void AddStopsToGraph(Graph& stops_graph);
	void AddBusesToGraph(Graph& stops_graph);

private:
	// время ожидания автобуса на остановке, в минутах
	int bus_wait_time_ = 0;

	// скорость автобуса, в км/ч
	double bus_velocity_ = 0.0;

	Graph graph_;
	
	Router router_;

	std::unordered_map<std::string, graph::VertexId> stop_name_to_vertex_id_;

	TransportCatalogue catalogue_;

};