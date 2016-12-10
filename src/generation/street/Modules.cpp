//
// Created by rdelfin on 12/10/16.
//

#include "generation/street/Modules.h"

#include "generation/street/Attributes.h"


Module::Module() {

}

void Module::addAttribute(Attribute* attribute) {
    attributes.push_back(attribute);
}

Module::~Module() {
    for(Attribute* attribute : attributes) {
        delete attribute;
    }
}

RoadModule::RoadModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute) {
    this->addAttribute(delayAttribute);
    this->addAttribute(ruleAttribute);
}

const DelayAttribute* RoadModule::getDelayAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<DelayAttribute*>(at)) {
            return (DelayAttribute*)at;
        }
    }

    return nullptr;
}

const RuleAttribute* RoadModule::getRuleAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<RuleAttribute*>(at)) {
            return (RuleAttribute*)at;
        }
    }

    return nullptr;
}

RoadModule::~RoadModule() {

}

BranchModule::BranchModule(DelayAttribute* delayAttribute, RuleAttribute* ruleAttribute, RoadAttribute* roadAttribute) {
    addAttribute(delayAttribute);
    addAttribute(ruleAttribute);
    addAttribute(roadAttribute);
}

const DelayAttribute* BranchModule::getDelayAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<DelayAttribute*>(at)) {
            return (DelayAttribute*)at;
        }
    }

    return nullptr;
}

const RuleAttribute* BranchModule::getRuleAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<RuleAttribute*>(at)) {
            return (RuleAttribute*)at;
        }
    }

    return nullptr;
}
const RoadAttribute* BranchModule::getRoadAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<RoadAttribute*>(at)) {
            return (RoadAttribute*)at;
        }
    }

    return nullptr;
}

virtual ~BranchModule();

InquiryModule::InquiryModule(RoadAttribute* roadAttribute, StateAttribute* stateAttribute) {
    this->addAttribute(roadAttribute);
    this->addAttribute(stateAttribute);
}

const RoadAttribute* InquiryModule::getRoadAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<RoadAttribute*>(at)) {
            return (RoadAttribute*)at;
        }
    }

    return nullptr;
}

const StateAttribute* InquiryModule::getStateAttribute() {
    for(Attribute* at : attributes) {
        if(dynamic_cast<StateAttribute*>(at)) {
            return (StateAttribute*)at;
        }
    }

    return nullptr;
}

InquiryModule::~InquiryModule() {
    
}