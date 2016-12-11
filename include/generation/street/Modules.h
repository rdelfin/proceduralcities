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
    Module(const Module&);

    void addAttribute(Attribute*);

    virtual ~Module();

    std::vector<Attribute*> attributes;
};


class RoadModule : public Module {
public:
    RoadModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute);
    RoadModule(const RoadModule& mod) : Module(mod) { }

    const DelayAttribute* getDelayAttribute();
    const RuleAttribute* getRuleAttribute();

    virtual ~RoadModule();
private:
};

class InquiryModule : public Module {
public:
    InquiryModule(RoadAttribute* roadAttribute, StateAttribute* stateAttribute);
    InquiryModule(const InquiryModule& mod) : Module(mod) { }

    const RoadAttribute* getRoadAttribute();
    const StateAttribute* getStateAttribute();

    virtual ~InquiryModule();
private:
};

class BranchModule : public Module {
public:
    BranchModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute, RoadAttribute* roadAttribute);
    BranchModule(const BranchModule& mod) : Module(mod) { }

    const DelayAttribute* getDelayAttribute();
    const RuleAttribute* getRuleAttribute();
    const RoadAttribute* getRoadAttribute();

    virtual ~BranchModule();
};

class DrawnRoadModule : public  Module {
public:
    DrawnRoadModule(RoadAttribute* roadAttribute);
    DrawnRoadModule(const DrawnRoadModule& mod) : Module(mod) { }

    const RoadAttribute* getRoadAttribute();

    virtual ~DrawnRoadModule() { }
};

class TerminationModule : public Module {
public:
    TerminationModule() { }
    TerminationModule(const TerminationModule& mod) : Module(mod) { }
    virtual ~TerminationModule() { }
private:
};

class StartModule : public Module {
public:
    StartModule() { }
    StartModule(const StartModule& mod) : Module(mod) {  }
    virtual ~StartModule() { }
};

class EndModule : public Module {
public:
    EndModule() { }
    EndModule(const EndModule& mod) : Module(mod) { }
    virtual ~EndModule() { }
};

#endif //PROCEDURALCITIES_MODULES_H
