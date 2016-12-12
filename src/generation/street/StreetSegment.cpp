//
// Created by rdelfin on 12/9/16.
//

#include <generation/street/StreetSegment.h>
#include <generation/street/Intersection.h>

StreetSegment::StreetSegment()
    : waypoints(), highway(false), start(nullptr), end(nullptr) {

}

StreetSegment::StreetSegment(const std::vector<glm::vec2>& waypoints, Intersection* start, Intersection* end, bool highway)
    : waypoints(waypoints), highway(highway), start(start), end(end) {
    if(start != nullptr) start->addStartingStreet(this);
    if(end != nullptr) end->addEndingStreet(this);

}

glm::vec2 StreetSegment::get(float val) {
    if(val < 0.0f) {
        val = 0.0f;
    } else if(val > 1.0f) {
        val = 1.0f;
    }

    int segment = int(val * (waypoints.size() - 1));
    float diff = val - int(val * waypoints.size());

    if(segment == waypoints.size() - 1) return waypoints[segment];

    return waypoints[segment] + diff*(waypoints[segment + 1] - waypoints[segment]);
}

void StreetSegment::addLines(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, float width, float y) {

    size_t startIdx;
    for(size_t i = 0; i < waypoints.size(); i++) {
        startIdx = vertices.size();

        glm::vec2 pointVector = (i == 0 ? glm::normalize(waypoints[i + 1] - waypoints[i]) : glm::normalize(waypoints[i] - waypoints[i - 1]));
        glm::vec2 rightVector = glm::normalize(glm::vec2(pointVector.y, -pointVector.x));  // 2D cross product

        glm::vec2 leftCorner = waypoints[i] - (width/2)*rightVector;
        glm::vec2 rightCorner = waypoints[i] + (width/2)*rightVector;

        vertices.push_back(glm::vec4(leftCorner.x, y, leftCorner.y, 1.0f));
        vertices.push_back(glm::vec4(rightCorner.x, y, rightCorner.y, 1.0f));

        if(i > 0) {
            faces.push_back(glm::uvec3(startIdx - 2, startIdx - 1, startIdx));
            faces.push_back(glm::uvec3(startIdx - 1, startIdx + 1, startIdx));
        }
    }

}

bool StreetSegment::collides(const StreetSegment& segment, glm::vec3& intersectionPoint) {
    if(*this == segment) {
        return false;
    }

    //for(auto i = 0; i <  )
}

bool StreetSegment::operator==(const StreetSegment& rhs) {
    return this->highway == rhs.highway
            && this->start == rhs.start
            && this->end == rhs.end
            && this->waypoints == rhs.waypoints;
}

StreetSegment::~StreetSegment() {

}