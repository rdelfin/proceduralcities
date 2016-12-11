//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/GlobalGoals.h>
#include <glm/vec2.hpp>
#include <cmath>

#include <StreetMap.h>
#include <iostream>

#define PI 3.14159265f

// Useful function on [0, 2pi)
float constrainAngle(float x){
    x = x * 360.0f / (2*PI);
    x = fmod(x,360);
    if (x < 0)
        x += 360;
    return x * 2 * PI / 360.0f;
}

float angleDiff(float x, float y) {
    float result = atan2(sin(x - y), cos(x - y));
    return constrainAngle(result);
}

GlobalGoals::GlobalGoals() {

}

void GlobalGoals::operator()(const RoadAttribute& roadAttribute, const RuleAttribute& ruleAttribute,
                std::vector<DelayAttribute>& pDel, std::vector<RuleAttribute>& pRuleAttr, std::vector<RoadAttribute>& pRoadAttr) {
    getAttribs(roadAttribute, ruleAttribute, pDel, pRuleAttr, pRoadAttr);
}

void GlobalGoals::getAttribs(const RoadAttribute& roadAttribute, const RuleAttribute& ruleAttribute,
                std::vector<DelayAttribute>& pDel, std::vector<RuleAttribute>& pRuleAttr, std::vector<RoadAttribute>& pRoadAttr) {
    if(ruleAttribute.pattern == ROAD_RECTANGULAR) {
        glm::vec2 roadEnd = roadAttribute.end();

        float widthAngleDiff = min(abs(angleDiff(roadAttribute.angle, ruleAttribute.initialAngle)),
                                   abs(angleDiff(roadAttribute.angle, constrainAngle(ruleAttribute.initialAngle + PI))));

        if(widthAngleDiff != 0) {
            //std::cout << "OH SHIT IT'S " << widthAngleDiff << std::endl;
        }


        bool isGridStreet = (widthAngleDiff < 0.1 || abs(widthAngleDiff - PI/2) < 0.1);

        pDel.push_back(isGridStreet ? DelayAttribute(1) : DelayAttribute(-1));
        pDel.push_back(isGridStreet ? DelayAttribute(1) : DelayAttribute(-1));
        pDel.push_back(isGridStreet ? DelayAttribute(1) : DelayAttribute(-1));
        pRuleAttr.push_back(ruleAttribute);
        pRuleAttr.push_back(ruleAttribute);
        pRuleAttr.push_back(ruleAttribute);

        bool wideStreet = widthAngleDiff < 45;
        float roadLength = wideStreet ? ruleAttribute.width : ruleAttribute.height;
        float parallelLength = wideStreet ? ruleAttribute.height : ruleAttribute.width;

        pRoadAttr.push_back(RoadAttribute(roadLength, constrainAngle(roadAttribute.angle), roadEnd));
        pRoadAttr.push_back(RoadAttribute(parallelLength, constrainAngle(roadAttribute.angle + PI/2), roadEnd));
        pRoadAttr.push_back(RoadAttribute(parallelLength, constrainAngle(roadAttribute.angle - PI/2), roadEnd));
    }
}

GlobalGoals::~GlobalGoals() {

}