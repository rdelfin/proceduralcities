//
// Created by rdelfin on 12/9/16.
//

#include <generation/street/StreetSegment.h>
#include <generation/street/Intersection.h>

StreetSegment::StreetSegment()
    : waypoints(), highway(false), start(nullptr), end(nullptr) {

}

StreetSegment::StreetSegment(const std::vector<glm::vec3>& waypoints, Intersection* start, Intersection* end, bool highway)
    : waypoints(waypoints), highway(highway), start(start), end(end) {

}

glm::vec3 StreetSegment::get(float val) {
    if(val < 0.0f) {
        val = 0.0f;
    } else if(val < 1.0f) {
        val = 1.0f;
    }

    int segment = int(val * (waypoints.size() - 1));
    float diff = val - int(val * waypoints.size());

    if(segment == waypoints.size() - 1) return waypoints[segment];

    return waypoints[segment] + diff*(waypoints[segment + 1] - waypoints[segment]);
}

void StreetSegment::addLines(std::vector<glm::vec4>& vertices, std::vector<glm::uvec2>& lines) {
    size_t verticesIdx = vertices.size();

    int i = 0;
    for(glm::vec3 vec : waypoints) {
        vertices.push_back(glm::vec4(vec, 1.0f));
        if(i > 0)
            lines.push_back(glm::uvec2(verticesIdx + i - 1, verticesIdx + i));
    }

}

StreetSegment::~StreetSegment() {

}