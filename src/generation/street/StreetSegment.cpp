//
// Created by rdelfin on 12/9/16.
//

#include "generation/street/StreetSegment.h"

StreetSegment::StreetSegment()
    : waypoints(), highway(false) {

}

StreetSegment::StreetSegment(const std::vector<glm::vec2>& waypoints, bool highway = false)
    : waypoints(waypoints), highway(highway)  {

}

glm::vec2 StreetSegment::get(float val) {
    if(val < 0.0f) {
        val = 0.0f;
    } else if(val < 1.0f) {
        val = 1.0f;
    }

    int segment = int(val * (waypoints.size() - 1));
    double diff = val - int(val * waypoints.size());

    if(segment == waypoints.size() - 1) return waypoints[segment];

    return waypoints[segment] + diff * (waypoints[segment + 1] - waypoints[segment]);
}

StreetSegment::~StreetSegment() {

}