//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_LOCALCONSTRAINTS_H
#define PROCEDURALCITIES_LOCALCONSTRAINTS_H

#include <generation/street/LocalConstraints.h>
#include <generation/street/Attributes.h>

class LocalConstraints {
public:
    LocalConstraints();

    void operator()(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState);
    void getAttributes(const RoadAttribute& roadAttributes, RoadAttribute& newRoadAttributes, StateAttribute& newState);

    ~LocalConstraints();
private:
};


#endif //PROCEDURALCITIES_LOCALCONSTRAINTS_H
