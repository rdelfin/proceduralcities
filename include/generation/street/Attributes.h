//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_ATTRIBUTES_H
#define PROCEDURALCITIES_ATTRIBUTES_H

#include <glm/glm.hpp>
#include <StreetMap.h>

enum {
    STATE_UNASSIGNED,
    STATE_SUCCEEDED,
    STATE_FAILED
} typedef State;


struct Attribute {
    virtual ~Attribute() { }
};

struct RoadAttribute : public Attribute {
    RoadAttribute(float length, float angle, glm::vec2 start) : length(length), angle(angle), start(start) { }

    float length;
    float angle; // In radians
    glm::vec2 start;
    glm::vec2 end() const { return start + length*glm::vec2(cos(angle), sin(angle)); }

    virtual ~RoadAttribute() { }
};

struct RuleAttribute : public Attribute {
    // Constructor for rectangular
    RuleAttribute(float angle, float width, float height)
        : initialAngle(angle), width(width), height(height), pattern(ROAD_RECTANGULAR) {
    }

    RoadPattern pattern;
    float initialAngle, width, height;

    virtual ~RuleAttribute() { }
};

struct StateAttribute : public Attribute {
    StateAttribute(State state) : state(state) { }

    virtual ~StateAttribute() { }

    State state;
};

struct DelayAttribute : public Attribute {
    DelayAttribute(int delay) : delay(delay) { }

    virtual ~DelayAttribute() { }

    int delay;
};

#endif //PROCEDURALCITIES_ATTRIBUTES_H
