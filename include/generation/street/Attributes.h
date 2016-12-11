//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_ATTRIBUTES_H
#define PROCEDURALCITIES_ATTRIBUTES_H

#include <glm/glm.hpp>

enum {
    STATE_UNASSIGNED,
    STATE_SUCCEEDED,
    STATE_FAILED
} typedef State;


struct Attribute {
    virtual Attribute* copy() const = 0;

    virtual ~Attribute() { }
};

struct RoadAttribute : public Attribute {
    RoadAttribute(float length, float angle, glm::vec2 start) : length(length), angle(angle), start(start) { }

    virtual Attribute* copy() const {
        return new RoadAttribute(*this);
    }

    float length;
    float angle; // In radians
    glm::vec2 start;
    glm::vec2 end() const { return start + length*glm::vec2(cos(angle), sin(angle)); }

    virtual ~RoadAttribute() { }
};

struct RuleAttribute : public Attribute {
    virtual ~RuleAttribute() { }

    virtual Attribute* copy() const {
        return new RuleAttribute(*this);
    }
};

struct RectangleRuleAttribute : public RuleAttribute {
    RectangleRuleAttribute(float initialAngle, float width, float height)
        : initialAngle(initialAngle), width(width), height(height) {

    }

    virtual Attribute* copy() const {
        return new RectangleRuleAttribute(*this);
    }

    virtual ~RectangleRuleAttribute() { }

    float initialAngle;
    float width, height;
};

struct StateAttribute : public Attribute {
    StateAttribute(State state) : state(state) { }

    virtual Attribute* copy() const {
        return new StateAttribute(*this);
    }

    virtual ~StateAttribute() { }

    State state;
};

struct DelayAttribute : public Attribute {
    DelayAttribute(int delay) : delay(delay) { }

    virtual Attribute* copy() const {
        return new DelayAttribute(*this);
    }

    virtual ~DelayAttribute() { }

    int delay;
};

#endif //PROCEDURALCITIES_ATTRIBUTES_H
