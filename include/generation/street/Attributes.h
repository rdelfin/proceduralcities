//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_ATTRIBUTES_H
#define PROCEDURALCITIES_ATTRIBUTES_H

enum {
    STATE_UNASSIGNED,
    STATE_SUCCEEDED,
    STATE_FAILED
} typedef State;


struct Attribute {
    virtual ~Attribute() { }
};

struct RoadAttribute : public Attribute {
    float length;
    float angle; // In radians

    virtual ~RoadAttribute() { }
};

struct RuleAttribute : public Attribute {
    virtual ~RuleAttribute() { }
};

struct StateAttribute : public Attribute {
    virtual ~StateAttribute() { }

    State state;
};

struct DelayAttribute : public Attribute {
    virtual ~DelayAttribute() { }

    float delay;
};

#endif //PROCEDURALCITIES_ATTRIBUTES_H
