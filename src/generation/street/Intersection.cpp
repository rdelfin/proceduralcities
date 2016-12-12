//
// Created by rdelfin on 12/9/16.
//

#include <generation/street/Intersection.h>
#include <generation/street/StreetSegment.h>

Intersection::Intersection(glm::vec2 position)
    : position(position), streets() {

}

Intersection::Intersection(const Intersection& intersection)
    : position(intersection.position), streets(intersection.streets) {

}

bool Intersection::addStartingStreet(StreetSegment* s) {
    if(s != nullptr && glm::length(s->get(0.0) - position) < 0.001) {
        streets.push_back(s);
        return true;
    }

    return false;
}

bool Intersection::addEndingStreet(StreetSegment* s) {
    if(s != nullptr && glm::length(s->get(1.0) - position) < 0.001) {
        streets.push_back(s);
        return true;
    }

    return false;
}

glm::vec2 Intersection::getPosition() {
    return position;
}

const std::vector<StreetSegment*>& Intersection::getStreets() {
    return  streets;
}


Intersection::~Intersection() {

}
