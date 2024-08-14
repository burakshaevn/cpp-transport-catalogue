/*
* Êîä, îòâå÷àþùèé çà âèçóàëèçàöèþ êàðòû ìàðøðóòîâ â ôîðìàòå SVG. 
*/

#pragma once

#include "svg.h"
#include "geo.h"
#include "json.h"
#include "domain.h"

#include <algorithm>

namespace renderer {

    inline const double EPSILON = 1e-6; 

    class SphereProjector {
    public: 
        template <typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
            double max_width, double max_height, double padding)
            : padding_(padding) //
        {
            // Åñëè òî÷êè ïîâåðõíîñòè ñôåðû íå çàäàíû, âû÷èñëÿòü íå÷åãî
            if (points_begin == points_end) {
                return;
            }

            // Íàõîäèì òî÷êè ñ ìèíèìàëüíîé è ìàêñèìàëüíîé äîëãîòîé
            const auto [left_it, right_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
            min_lon_ = left_it->lng;
            const double max_lon = right_it->lng;

            // Íàõîäèì òî÷êè ñ ìèíèìàëüíîé è ìàêñèìàëüíîé øèðîòîé
            const auto [bottom_it, top_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
            const double min_lat = bottom_it->lat;
            max_lat_ = top_it->lat;

            // Âû÷èñëÿåì êîýôôèöèåíò ìàñøòàáèðîâàíèÿ âäîëü êîîðäèíàòû x
            std::optional<double> width_zoom;
            if (!(std::abs(max_lon - min_lon_) < EPSILON)) { 
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            // Âû÷èñëÿåì êîýôôèöèåíò ìàñøòàáèðîâàíèÿ âäîëü êîîðäèíàòû y
            std::optional<double> height_zoom;
            if (!(std::abs(max_lon - min_lon_) < EPSILON)) {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom) {
                // Êîýôôèöèåíòû ìàñøòàáèðîâàíèÿ ïî øèðèíå è âûñîòå íåíóëåâûå,
                // áåð¸ì ìèíèìàëüíûé èç íèõ
                zoom_coeff_ = std::min(*width_zoom, *height_zoom);
            }
            else if (width_zoom) {
                // Êîýôôèöèåíò ìàñøòàáèðîâàíèÿ ïî øèðèíå íåíóëåâîé, èñïîëüçóåì åãî
                zoom_coeff_ = *width_zoom;
            }
            else if (height_zoom) {
                // Êîýôôèöèåíò ìàñøòàáèðîâàíèÿ ïî âûñîòå íåíóëåâîé, èñïîëüçóåì åãî
                zoom_coeff_ = *height_zoom;
            }
        }

        // Ïðîåöèðóåò øèðîòó è äîëãîòó â êîîðäèíàòû âíóòðè SVG-èçîáðàæåíèÿ
        svg::Point operator()(detail::Coordinates coords) const {
            return {
                (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_
            };
        }

    private:
        double padding_;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };

    struct RenderSettings {
        double width = 0.0;
        double height = 0.0;
        double padding = 0.0;
        double line_width = 0.0;
        double stop_radius = 0.0;
        int bus_label_font_size = 0;
        svg::Point bus_label_offset = { 0.0, 0.0 };
        int stop_label_font_size = 0;
        svg::Point stop_label_offset = { 0.0, 0.0 };
        svg::Color underlayer_color = { svg::NoneColor };
        double underlayer_width = 0.0;
        std::vector<svg::Color> color_palette{};
    };

    class MapRenderer {
    public:
        MapRenderer(const RenderSettings& render_settings)
            : render_settings_(render_settings)
        {}

        std::vector<svg::Polyline> GetRouteLines(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const;

        std::vector<svg::Text> GetBusLabel(const std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const;

        std::vector<svg::Circle> GetStopsSymbols(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const;

        std::vector<svg::Text> GetStopsLabels(const std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const;

        svg::Document GetSVG(const std::map<std::string_view, const Bus*>& buses) const; 

        void DrawRouteLines(const std::map<std::string_view, const Bus* >& buses, SphereProjector& sp, svg::Document& result) const;
        void DrawBusLabel(const std::map<std::string_view, const Bus* >& buses, SphereProjector& sp, svg::Document& result) const;
        void DrawStopsSymbols(std::map<std::string_view, const Stop*>& all_stops, SphereProjector& sp, svg::Document& result) const;
        void DrawStopsLabels(std::map<std::string_view, const Stop*>& all_stops, SphereProjector& sp, svg::Document& result) const;

    private:
        const RenderSettings render_settings_;
    };

} // namespace renderer
