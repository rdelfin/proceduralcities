//
// Created by rdelfin on 12/9/16.
//

#include <generation/street/Intersection.h>
#include <generation/street/StreetSegment.h>

Intersection::Intersection(glm::vec3 position)
    : position(position) {

}

bool Intersection::addStartingStreet(StreetSegment* s) {
    if(s->get(0.0) == position) {
        streets.push_back(s);
        return true;
    }

    return false;
}

bool Intersection::addEndingStreet(StreetSegment* s) {
    if(s->get(1.0) == position) {
        streets.push_back(s);
        return true;
    }

    return false;
}

glm::vec3 Intersection::getPosition() {
    return position;
}

const std::vector<StreetSegment*>& Intersection::getStreets() {
    return  streets;
}


Intersection::~Intersection() {

}
