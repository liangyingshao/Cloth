#include "spring.h"
#include<iostream>
extern float radius;

using namespace std;

Spring::Spring()
{
    //ctor
}

Spring::~Spring()
{
    //dtor
}

Spring::Spring(Mass* mass1, Mass* mass2,
    float springConstant, float springLength,
    float frictionConstant)
{
    this->springConstant = springConstant;
    this->restLength = springLength;
    this->frictionConstant = frictionConstant;

    this->mass1 = mass1;
    this->mass2 = mass2;
}

//计算弹簧的两个质点所受的阻尼力和弹力
void Spring::solve()
{
    glm::vec3 springVector = mass1->getPos() - mass2->getPos();                 // 弹簧对应的向量
    float r = glm::length(springVector);                                        // 弹簧长度

    glm::vec3 force(0);// 
    if (r != 0)
    {
        force += (springVector / r) * (r - restLength) * (-springConstant);     // mass1受到的弹力
        //          方向                  delta L             -弹性系数
    }
    force += -(mass1->getVel() - mass2->getVel()) * frictionConstant;           // 与相对速度反向的阻尼力，相对速度越大阻尼力越大，以免永远震荡
    //          mass1对mass2的相对速度                阻尼系数
    mass1->applyForce(force);                                                   // 将力作用于mass1
    mass2->applyForce(-force);                                                  // -force作用于mass2
}