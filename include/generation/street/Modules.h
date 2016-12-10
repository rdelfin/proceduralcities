//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_MODULES_H
#define PROCEDURALCITIES_MODULES_H

#include <vector>
#include <generation/street/Attributes.h>

/**
 * Any object in the L-system that can be replaced by a rule
 */
class Module {
public:
    Module();

    void addAttribute(Attribute*);

    virtual ~Module();

private:
    std::vector<Attribute*> attributes;
};


class RoadModule : public Module {
public:
    RoadModule(DelayAttribute* delayAttribute, RoadAttribute* roadAttribute);

    virtual ~RoadModule();
private:
};

class InquiryModule : public Module {
public:
    InquiryModule(RoadAttribute* roadAttribute, StateAttribute* stateAttribute);

    virtual ~InquiryModule();
private:
};


#endif //PROCEDURALCITIES_MODULES_H
