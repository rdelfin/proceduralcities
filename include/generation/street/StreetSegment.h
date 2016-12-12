//
// Created by rdelfin on 12/9/16.
//

#ifndef PROCEDURALCITIES_STREETSEGMENT_H
#define PROCEDURALCITIES_STREETSEGMENT_H

#include <glm/glm.hpp>
#include <vector>

class Intersection;

class StreetSegment {
public:
    StreetSegment();
    StreetSegment(const std::vector<glm::vec3>& waypoints, Intersection* start, Intersection* end, bool highway = false);

    glm::vec3 get(float val);

    void addLines(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, glm::vec3 up, float width);

    ~StreetSegment();

    std::vector<glm::vec3> waypoints;
    bool highway;

    Intersection *start, *end;
};


#endif //PROCEDURALCITIES_STREETSEGMENT_H
