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
    Module() { }

    virtual Module* copy() = 0;

    virtual ~Module() { }
};


class RoadModule : public Module {
public:
    RoadModule(const DelayAttribute& delayAttribute, const RuleAttribute& ruleAttribute)
            : delayAttribute(delayAttribute), ruleAttribute(ruleAttribute) { }

    virtual Module* copy() {
        return new RoadModule(delayAttribute, ruleAttribute);
    }

    DelayAttribute delayAttribute;
    RuleAttribute ruleAttribute;

    virtual ~RoadModule() { }
private:
};

class InquiryModule : public Module {
public:
    InquiryModule(const RoadAttribute& roadAttribute, const StateAttribute& stateAttribute) :roadAttribute(roadAttribute), stateAttribute(stateAttribute) { }

    virtual Module* copy() {
        return new InquiryModule(roadAttribute, stateAttribute);
    }

    RoadAttribute roadAttribute;
    StateAttribute stateAttribute;

    virtual ~InquiryModule() { }
private:
};

class BranchModule : public Module {
public:
    BranchModule(const DelayAttribute& delayAttribute, const RuleAttribute& ruleAttribute, const RoadAttribute& roadAttribute)
            : delayAttribute(delayAttribute), ruleAttribute(ruleAttribute), roadAttribute(roadAttribute) { }

    virtual Module* copy() {
        return new BranchModule(delayAttribute, ruleAttribute, roadAttribute);
    }

    DelayAttribute delayAttribute;
    RuleAttribute ruleAttribute;
    RoadAttribute roadAttribute;

    virtual ~BranchModule() { }
};

class DrawnRoadModule : public  Module {
public:
    DrawnRoadModule(RoadAttribute& roadAttribute) : roadAttribute(roadAttribute) { }

    virtual Module* copy() {
        return new DrawnRoadModule(roadAttribute);
    }

    RoadAttribute roadAttribute;

    virtual ~DrawnRoadModule() { }
};

class StartModule : public Module {
public:
    StartModule() { }

    virtual Module* copy() {
        return new StartModule();
    }

    virtual ~StartModule() { }
};

class EndModule : public Module {
public:
    EndModule() { }

    virtual Module* copy() {
        return new EndModule();
    }

    virtual ~EndModule() { }
};

#endif //PROCEDURALCITIES_MODULES_H
