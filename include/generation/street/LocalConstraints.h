//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_LOCALCONSTRAINTS_H
#define PROCEDURALCITIES_LOCALCONSTRAINTS_H

#include <generation/street/LocalConstraints.h>
#include <generation/street/Attributes.h>

#include <map>
#include <set>

class LocalConstraints {
public:
    LocalConstraints(std::map<float, std::set<float>> waterPoints, std::map<float, std::set<float>> parksPoints);

    void operator()(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState);
    void getAttributes(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState);

    ~LocalConstraints();
private:
    bool collidesWithEnvironment(glm::vec2 start, glm::vec2 end);
    std::map<float, std::set<float>> waterPoints;
    std::map<float, std::set<float>> parksPoints;

    float minLength, maxDeltaAngle;
};


#endif //PROCEDURALCITIES_LOCALCONSTRAINTS_H
