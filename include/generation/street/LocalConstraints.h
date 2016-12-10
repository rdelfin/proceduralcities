//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_LOCALCONSTRAINTS_H
#define PROCEDURALCITIES_LOCALCONSTRAINTS_H

#include <generation/street/LocalConstraints.h>

class LocalConstraints {
public:
    PublicConstraints();

    void getAttributes(const RoadAttributes& roadAttributes, RoadAttributes& newRoadAttributes, State& newState);

    ~PublicConstraints();
private:
};


#endif //PROCEDURALCITIES_LOCALCONSTRAINTS_H
