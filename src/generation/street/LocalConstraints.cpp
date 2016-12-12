//
// Created by rdelfin on 12/10/16.
//

#include <map>
#include <set>

#include <generation/street/LocalConstraints.h>
#include <generation/street/Attributes.h>
#include <generation/street/StreetSegment.h>
#include <generation/street/Intersection.h>
#include <vector>

#define PI 3.14159265f

LocalConstraints::LocalConstraints(std::map<float, std::set<float>> waterPoints, std::map<float, std::set<float>> parksPoints)
    : waterPoints(waterPoints), parksPoints(parksPoints), minLength(1.0f), maxDeltaAngle(PI / 2) {

}

void LocalConstraints::operator()(const RoadAttribute& roadAttributes, const std::vector<StreetSegment*>& streets, const std::vector<Intersection*>& intersections,
                                  RoadAttribute& newRoadAttributes, StateAttribute& newState) {
    getAttributes(roadAttributes, streets, intersections, newRoadAttributes, newState);
}

void LocalConstraints::getAttributes(const RoadAttribute& roadAttributes, const std::vector<StreetSegment*>& streets, const std::vector<Intersection*>& intersections,
                                     RoadAttribute& newRoadAttributes, StateAttribute& newState) {
    newRoadAttributes = roadAttributes;

    // Merge with streets and intersections
    // TODO: Intersections


    // Check street colission with the environment
    bool streetFound = false;
    float lengthDelta = 0.0f;
    float angleDelta = 0.0f;
    while(!streetFound) {
        newRoadAttributes.length = roadAttributes.length - lengthDelta;
        newRoadAttributes.angle = roadAttributes.angle + angleDelta;


        streetFound = !collidesWithEnvironment(newRoadAttributes.start, newRoadAttributes.end());
        if(streetFound) break;

        newRoadAttributes.angle = roadAttributes.angle - angleDelta;
        streetFound = !collidesWithEnvironment(newRoadAttributes.start, newRoadAttributes.end());

        lengthDelta += 0.1f;
        if(roadAttributes.length - lengthDelta < minLength) {
//            lengthDelta = 0.0f;
//            angleDelta += 0.01f;
//
//            if(angleDelta > maxDeltaAngle)
                break;
        }
    }

    newState = streetFound ? STATE_SUCCEEDED : STATE_FAILED;
}

bool LocalConstraints::collidesWithEnvironment(glm::vec2 start, glm::vec2 end) {
    for(float i = 0; i <= 1.0f; i += 0.01f) {
        glm::vec2 point = start + i*(end - start);

        float x = round(point.x);
        float z = round(point.y);
        if (waterPoints.count(x) != 0 && waterPoints[x].count(z) != 0) {
            return true;
        } else if (parksPoints.count(x) != 0 && parksPoints[x].count(z) != 0) {
            return true;
        } else if (x <= -100.0f || x >= 100.0f || z <= -100.0f || z >= 100.0f) {
            return true;
       }
    }
}

LocalConstraints::~LocalConstraints() {

}