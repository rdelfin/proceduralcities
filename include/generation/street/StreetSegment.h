//
// Created by rdelfin on 12/9/16.
//

#ifndef PROCEDURALCITIES_STREETSEGMENT_H
#define PROCEDURALCITIES_STREETSEGMENT_H

#include <glm/glm.hpp>

class StreetSegment {
public:
    StreetSegment();
    StreetSegment(const std::vector<glm::vec2>& waypoints, bool highway = false);

    glm::vec2 get(float val);

    ~StreetSegment();
private:
    std::vector<glm::vec2> waypoints;
    bool highway = false;
};


#endif //PROCEDURALCITIES_STREETSEGMENT_H
