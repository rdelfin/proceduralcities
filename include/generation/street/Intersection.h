//
// Created by rdelfin on 12/9/16.
//

#ifndef PROCEDURALCITIES_INTERSECTION_H
#define PROCEDURALCITIES_INTERSECTION_H

#include <vector>
#include <glm/glm.hpp>

class StreetSegment;

class Intersection {
public:
    Intersection(glm::vec3 position);

    bool addStartingStreet(StreetSegment*);
    bool addEndingStreet(StreetSegment*);
    glm::vec3 getPosition();
    const std::vector<StreetSegment*>& getStreets();

    ~Intersection();

private:
    std::vector<StreetSegment*> streets;
    glm::vec3 position;
};


#endif //PROCEDURALCITIES_INTERSECTION_H
