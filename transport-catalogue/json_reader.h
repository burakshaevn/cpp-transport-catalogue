/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

#pragma once

#include "json.h"
#include "transport_catalogue.h"

#include "map_renderer.h"
#include "request_handler.h"
#include "json_builder.h"
#include <iostream>
#include <sstream>

class RequestHandler;

class JsonReader {
public:
    JsonReader(std::istream& input)
        : input_(json::Load(input))
    {}

    const json::Node& GetBaseRequests() const;

    const json::Node& GetStatRequests() const;
    
    const json::Node& GetRenderSettings() const;

    void ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const;

    void PullCatalogue(TransportCatalogue& catalogue);

    void ProcessStopRequests(const json::Array& arr, TransportCatalogue& catalogue);

    void ProcessBusRequests(const json::Array& arr, TransportCatalogue& catalogue); 
    
    renderer::MapRenderer PullRenderSettings(const json::Dict& request_map) const;

    const json::Node PrintBus(const json::Dict& request_map, RequestHandler& rh) const;

    const json::Node PrintStop(const json::Dict& request_map, RequestHandler& rh) const;

    const json::Node PrintMap(const json::Dict& request_map, RequestHandler& rh) const;

    svg::Color PullColor(const json::Node& color_node) const;

private:
    json::Document input_; 

    std::tuple<std::string_view, detail::Coordinates, std::map<std::string_view, int>> PullStop(const json::Dict& request_map) const;
    
    void PullStopDistances(TransportCatalogue& catalogue) const;
    
    std::tuple<std::string_view, std::vector<const Stop*>, bool> PullBus(const json::Dict& request_map, TransportCatalogue& catalogue) const;
};