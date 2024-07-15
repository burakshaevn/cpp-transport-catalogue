/*
 * ��� ��������� �������� � ���� � ������������ ������� ������� � ������� JSON
 */

#include "json_reader.h"

//const json::Node& JsonReader::GetBaseRequests() const {
//    if (!input_.GetRoot().AsMap().count("base_requests")) {
//        return nullptr;
//    }
//    return input_.GetRoot().AsMap().at("base_requests");
//}

const json::Node& JsonReader::GetBaseRequests() const {
    const auto& root_map = input_.GetRoot().AsDict();
    auto it = root_map.find("base_requests");
    if (it == root_map.end()) { 
        static json::Node empty_node;  
        return empty_node;
    }
    return it->second;
}

const json::Node& JsonReader::GetStatRequests() const {
    //if (!input_.GetRoot().AsMap().count("stat_requests")) {
    //    return nullptr;
    //}
    //return input_.GetRoot().AsMap().at("stat_requests");
    const auto& root_map = input_.GetRoot().AsDict();
    auto it = root_map.find("stat_requests");
    if (it == root_map.end()) { 
        static json::Node empty_node; 
        return empty_node;
    }
    return it->second;
} 

const json::Node& JsonReader::GetRenderSettings() const {
    //if (!input_.GetRoot().AsMap().count("render_settings")) {
    //    return nullptr;
    //}
    //return input_.GetRoot().AsMap().at("render_settings");
    const auto& root_map = input_.GetRoot().AsDict();
    auto it = root_map.find("render_settings");
    if (it == root_map.end()) { 
        static json::Node empty_node;  
        return empty_node;
    }
    return it->second;
}

void JsonReader::ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const {
    json::Array result;
    for (auto& request : stat_requests.AsArray()) {
        const auto& request_map = request.AsDict();
        const auto& type = request_map.at("type").AsString();
        if (type == "Stop") {
            result.push_back(PrintStop(request_map, rh).AsDict());
        }
        if (type == "Bus") {
            result.push_back(PrintBus(request_map, rh).AsDict());
        }
        if (type == "Map") {
            result.push_back(PrintMap(request_map, rh).AsDict());
        }
    }

    json::Print(json::Document{ result }, std::cout);
}

//void JsonReader::PullCatalogue(TransportCatalogue& catalogue) {
//    const json::Array& arr = GetBaseRequests().AsArray();
//    for (auto& request_stops : arr) {
//        const auto& request_stops_map = request_stops.AsMap();
//        const auto& type = request_stops_map.at("type").AsString();
//        if (type == "Stop") {
//            auto [stop_name, coordinates, stop_distances] = PullStop(request_stops_map);
//            catalogue.PushStop(stop_name, coordinates);
//        }
//    }
//    PullStopDistances(catalogue);
//
//    for (auto& request_bus : arr) {
//        const auto& request_bus_map = request_bus.AsMap();
//        const auto& type = request_bus_map.at("type").AsString();
//        if (type == "Bus") {
//            auto [bus_number, stops, circular_route] = PullBus(request_bus_map, catalogue);
//            catalogue.PushBus(bus_number, stops, circular_route);
//        }
//    }
//}

void JsonReader::PullCatalogue(TransportCatalogue& catalogue) {
    const json::Array& arr = GetBaseRequests().AsArray();
    ProcessStopRequests(arr, catalogue);
    PullStopDistances(catalogue);
    ProcessBusRequests(arr, catalogue);
}

// � ������ �� ��������� ������ �����, ��� �� �������. �� ���� ������, � �� ������, ������ �� ��� ������������� ������. 
// ����� ��� �������, ��� �������, �� ������ ��������� ��� ������� ���������. ������ � ����� ��������������� �������, ����� �� ������ 
// ���. �������, ������� ����� ���������� ������ �� ����� ������� ���� ���.

void JsonReader::ProcessStopRequests(const json::Array& arr, TransportCatalogue& catalogue) {
    for (const auto& request_stops : arr) {
        const auto& request_stops_map = request_stops.AsDict();
        const auto& type = request_stops_map.at("type").AsString();
        if (type == "Stop") {
            auto [stop_name, coordinates, stop_distances] = PullStop(request_stops_map);
            catalogue.PushStop(stop_name, coordinates);
        }
    }
}

void JsonReader::ProcessBusRequests(const json::Array& arr, TransportCatalogue& catalogue) {
    for (const auto& request_bus : arr) {
        const auto& request_bus_map = request_bus.AsDict();
        const auto& type = request_bus_map.at("type").AsString();
        if (type == "Bus") {
            auto [bus_number, stops, circular_route] = PullBus(request_bus_map, catalogue);
            catalogue.PushBus(bus_number, stops, circular_route);
        }
    }
}

std::tuple<std::string_view, detail::Coordinates, std::map<std::string_view, int>> JsonReader::PullStop(const json::Dict& request_map) const {
    std::string_view stop_name = request_map.at("name").AsString();
    detail::Coordinates coordinates = { request_map.at("latitude").AsDouble(), request_map.at("longitude").AsDouble() };
    std::map<std::string_view, int> stop_distances;
    auto& distances = request_map.at("road_distances").AsDict();
    for (auto& [stop_name, dist] : distances) {
        stop_distances.emplace(stop_name, dist.AsInt());
    }
    return { stop_name, coordinates, stop_distances };
}

void JsonReader::PullStopDistances(TransportCatalogue& catalogue) const {
    const json::Array& arr = GetBaseRequests().AsArray();
    for (auto& request_stops : arr) {
        const auto& request_stops_map = request_stops.AsDict();
        const auto& type = request_stops_map.at("type").AsString();
        if (type == "Stop") {
            auto [stop_name, coordinates, stop_distances] = PullStop(request_stops_map);
            for (auto& [to_name, dist] : stop_distances) {
                auto from = catalogue.FindStop(stop_name);
                auto to = catalogue.FindStop(to_name);
                catalogue.SetDistance(from, to, dist);
            }
        }
    }
}

std::tuple<std::string_view, std::vector<const Stop*>, bool> JsonReader::PullBus(const json::Dict& request_map, TransportCatalogue& catalogue) const {
    std::string_view bus_number = request_map.at("name").AsString();
    std::vector<const Stop*> stops;
    for (auto& stop : request_map.at("stops").AsArray()) {
        stops.push_back(catalogue.FindStop(stop.AsString()));
    }
    bool circular_route = request_map.at("is_roundtrip").AsBool();

    return { bus_number, stops, circular_route };
}

renderer::MapRenderer JsonReader::PullRenderSettings(const json::Dict& request_map) const {
    renderer::RenderSettings render_settings;
    render_settings.width = request_map.at("width").AsDouble();
    render_settings.height = request_map.at("height").AsDouble();
    render_settings.padding = request_map.at("padding").AsDouble();
    render_settings.stop_radius = request_map.at("stop_radius").AsDouble();
    render_settings.line_width = request_map.at("line_width").AsDouble();
    render_settings.bus_label_font_size = request_map.at("bus_label_font_size").AsInt();
    const json::Array& bus_label_offset = request_map.at("bus_label_offset").AsArray();
    render_settings.bus_label_offset = { bus_label_offset[0].AsDouble(), bus_label_offset[1].AsDouble() };
    render_settings.stop_label_font_size = request_map.at("stop_label_font_size").AsInt();
    const json::Array& stop_label_offset = request_map.at("stop_label_offset").AsArray();
    render_settings.stop_label_offset = { stop_label_offset[0].AsDouble(), stop_label_offset[1].AsDouble() };

    //if (request_map.at("underlayer_color").IsString()) {
    //    render_settings.underlayer_color = request_map.at("underlayer_color").AsString();
    //}
    //else if (request_map.at("underlayer_color").IsArray()) {
    //    const json::Array& underlayer_color = request_map.at("underlayer_color").AsArray();
    //    if (underlayer_color.size() == 3) {
    //        render_settings.underlayer_color = svg::Rgb(underlayer_color[0].AsInt(), underlayer_color[1].AsInt(), underlayer_color[2].AsInt());
    //    }
    //    else if (underlayer_color.size() == 4) {
    //        render_settings.underlayer_color = svg::Rgba(underlayer_color[0].AsInt(), underlayer_color[1].AsInt(), underlayer_color[2].AsInt(), underlayer_color[3].AsDouble());
    //    }
    //    else {
    //        throw std::logic_error("wrong underlayer colortype");
    //    }
    //}
    //else {
    //    throw std::logic_error("wrong underlayer color");
    //}

    render_settings.underlayer_color = PullColor(request_map.at("underlayer_color"));

    render_settings.underlayer_width = request_map.at("underlayer_width").AsDouble();

    const json::Array& color_palette = request_map.at("color_palette").AsArray();
    for (const auto& color_element : color_palette) {
        if (color_element.IsString()) {
            render_settings.color_palette.emplace_back(color_element.AsString());
        }
        else if (color_element.IsArray()) {
            const json::Array& color_type = color_element.AsArray();
            if (color_type.size() == 3) {
                render_settings.color_palette.emplace_back(svg::Rgb(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt()));
            }
            else if (color_type.size() == 4) {
                render_settings.color_palette.emplace_back(svg::Rgba(color_type[0].AsInt(), color_type[1].AsInt(), color_type[2].AsInt(), color_type[3].AsDouble()));
            }
            else {
                throw std::logic_error("wrong color_palette type");
            }
        }
        else {
            throw std::logic_error("wrong color_palette");
        }
    }

    return render_settings;
} 

const json::Node JsonReader::PrintBus(const json::Dict& request_map, RequestHandler& rh) const {
    json::Dict result;
    const std::string& route_number = request_map.at("name").AsString();
    result["request_id"] = request_map.at("id").AsInt();
    if (!rh.GetCaralogue().FindBus(route_number)) {
        result["error_message"] = json::Node{ static_cast<std::string>("not found") };
    }
    else {
        const auto& bus = rh.GetBusStat(route_number);
        result["curvature"] = bus->curvature;
        result["route_length"] = bus->len;
        result["stop_count"] = static_cast<int>(bus->count_stops);
        result["unique_stop_count"] = static_cast<int>(bus->unique_count_stops);
    }
    return json::Node{ result };
}

const json::Node JsonReader::PrintStop(const json::Dict& request_map, RequestHandler& rh) const {
    json::Dict result;
    const std::string& stop_name = request_map.at("name").AsString();
    result["request_id"] = request_map.at("id").AsInt();
    if (!rh.GetCaralogue().FindStop(stop_name)) {
        result["error_message"] = std::string("not found");
    }
    else {
        json::Array buses;
        std::set<std::string> sorted_buses;
        for (const auto& bus : rh.GetCaralogue().GetBusesForStop(stop_name)) {
            sorted_buses.insert(bus->name);
        }
        json::Array buses_array;
        for (const auto& bus_name : sorted_buses) {
            buses_array.emplace_back(bus_name);
        }
        result["buses"] = buses_array;
    }
    return json::Node{ result };
}

const json::Node JsonReader::PrintMap(const json::Dict& request_map, RequestHandler& rh) const {
    json::Dict result;
    result["request_id"] = request_map.at("id").AsInt();
    std::ostringstream strm;
    svg::Document map = rh.RenderMap();
    map.Render(strm);
    result["map"] = strm.str();

    return json::Node{ result };
}

svg::Color JsonReader::PullColor(const json::Node& color_node) const {
    if (color_node.IsString()) {
        return color_node.AsString();  
    }
    else if (color_node.IsArray()) {
        const json::Array& color_array = color_node.AsArray();
        if (color_array.size() == 3) {
            return svg::Rgb(color_array[0].AsInt(), color_array[1].AsInt(), color_array[2].AsInt());
        }
        else if (color_array.size() == 4) {
            return svg::Rgba(color_array[0].AsInt(), color_array[1].AsInt(), color_array[2].AsInt(), color_array[3].AsDouble());
        }
        else {
            throw std::logic_error("Wrong color format in JSON");
        }
    }
    else {
        throw std::logic_error("Unexpected type for color in JSON");
    }
}