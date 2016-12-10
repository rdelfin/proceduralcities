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

protected:
    std::vector<Attribute*> attributes;
};


class RoadModule : public Module {
public:
    RoadModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute);

    const DelayAttribute* getDelayAttribute();
    const RuleAttribute* getRuleAttribute();

    virtual ~RoadModule();
private:
};

class InquiryModule : public Module {
public:
    InquiryModule(RoadAttribute* roadAttribute, StateAttribute* stateAttribute);

    const RoadAttribute* getRoadAttribute();
    const StateAttribute* getStateAttribute();

    virtual ~InquiryModule();
private:
};

class BranchModule : public Module {
public:
    BranchModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute, RoadAttribute* roadAttribute);

    const DelayAttribute* getDelayAttribute();
    const RuleAttribute* getRuleAttribute();
    const RoadAttribute* getRoadAttribute();

    virtual ~BranchModule();
};

class TerminationModule : public Module {
public:
    TerminationModule() { }
    virtual ~TerminationModule() { }
private:
};


#endif //PROCEDURALCITIES_MODULES_H
