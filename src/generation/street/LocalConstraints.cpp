//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/LocalConstraints.h>
#include <generation/street/Attributes.h>

LocalConstraints::LocalConstraints() {

}

void LocalConstraints::operator()(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState) {
    getAttributes(roadAttributes, newRoadAttributes, newState);
}

void LocalConstraints::getAttributes(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState) {

}

LocalConstraints::~LocalConstraints() {

}