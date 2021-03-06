//
// Created by rdelfin on 12/9/16.
//

#ifndef PROCEDURALCITIES_STREETSEGMENT_H
#define PROCEDURALCITIES_STREETSEGMENT_H

#include <glm/glm.hpp>
#include <vector>
#include <StreetMap.h>

class Intersection;

class StreetSegment {
public:
    StreetSegment();
    StreetSegment(const std::vector<glm::vec2>& waypoints, Intersection* start, Intersection* end, bool highway = false);

    glm::vec2 get(float val);

    void addLines(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, float width, float y);

    bool collides(const StreetSegment& segment, glm::vec2& intersectionPoint);

    bool operator==(const StreetSegment& rhs);

    StreetLine toStreetLine(float y) { return StreetLine(glm::vec4(waypoints[0].x, y, waypoints[0].y, 1.0f),
                                                         glm::vec4(waypoints[waypoints.size()-1].x, y, waypoints[waypoints.size()-1].y, 1.0f)); }

    ~StreetSegment();

    std::vector<glm::vec2> waypoints;
    bool highway;

    Intersection *start, *end;
};


#endif //PROCEDURALCITIES_STREETSEGMENT_H
