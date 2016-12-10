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

RoadModule::~RoadModule() {

}

InquiryModule::InquiryModule(RoadAttribute* roadAttribute, StateAttribute* stateAttribute) {
    this->addAttribute(roadAttribute);
    this->addAttribute(stateAttribute);
}

InquiryModule::~InquiryModule() {
    
}