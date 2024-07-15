#include <iostream>
#include <sstream>
#include "json_reader.h"
#include "request_handler.h" 
 
int main() {
    TransportCatalogue catalogue;
    
    JsonReader json_doc(std::cin);

    json_doc.PullCatalogue(catalogue);
     
    const auto& stat_requests = json_doc.GetStatRequests();
    const auto& render_settings = json_doc.GetRenderSettings().AsDict();
    const auto& renderer = json_doc.PullRenderSettings(render_settings);

    RequestHandler rh(catalogue, renderer);  

    json_doc.ProcessRequests(stat_requests, rh);
}