//
// Created by rdelfin on 12/10/16.
//

#include <map>
#include <set>

#include <generation/street/LocalConstraints.h>
#include <generation/street/Attributes.h>
#include <vector>

#define PI 3.14159265f

LocalConstraints::LocalConstraints(std::map<float, std::set<float>> waterPoints, std::map<float, std::set<float>> parksPoints)
    : waterPoints(waterPoints), parksPoints(parksPoints), minLength(1.0f), maxDeltaAngle(PI / 2) {

}

void LocalConstraints::operator()(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState) {
    getAttributes(roadAttributes, newRoadAttributes, newState);
}

void LocalConstraints::getAttributes(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState) {
    newRoadAttributes = roadAttributes;

    bool streetFound = false;
    double lengthDelta = 0.0f;
    double angleDelta = 0.0f;
    while(!streetFound) {
        newRoadAttributes.length -= roadAttributes.length - lengthDelta;
        newRoadAttributes.angle += roadAttributes.angle + angleDelta;

        streetFound = collidesWithEnvironment(newRoadAttributes.end());
        if(streetFound) break;

        newRoadAttributes.angle += roadAttributes.angle - angleDelta;
        streetFound = collidesWithEnvironment(newRoadAttributes.end());

        lengthDelta -= 0.1f;
        if(lengthDelta < minLength) {
            lengthDelta = 0.0f;
            angleDelta += 0.01f;

            if(angleDelta > maxDeltaAngle)
                break;
        }

        newState = streetFound ? STATE_SUCCEEDED : STATE_FAILED;
    }
}

bool LocalConstraints::collidesWithEnvironment(glm::vec2 point) {
    float x = (float) round(point.x);
    float z = (float) round(point.y);
    if (waterPoints.count(x) != 0 && waterPoints[x].count(z) != 0) {
        return false;
    }
    else if (parksPoints.count(x) != 0 && parksPoints[x].count(z) != 0) {
        return false;
    }
    else if (x <= -100.0f || x >= 100.0f || z <= -100.0f || z >= 100.0f) {
        return false;
    }

    return true;
}

LocalConstraints::~LocalConstraints() {

}