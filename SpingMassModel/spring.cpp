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

//���㵯�ɵ������ʵ����ܵ��������͵���
void Spring::solve()
{
    glm::vec3 springVector = mass1->getPos() - mass2->getPos();                 // ���ɶ�Ӧ������
    float r = glm::length(springVector);                                        // ���ɳ���

    glm::vec3 force(0);// 
    if (r != 0)
    {
        force += (springVector / r) * (r - restLength) * (-springConstant);     // mass1�ܵ��ĵ���
        //          ����                  delta L             -����ϵ��
    }
    force += -(mass1->getVel() - mass2->getVel()) * frictionConstant;           // ������ٶȷ����������������ٶ�Խ��������Խ��������Զ��
    //          mass1��mass2������ٶ�                ����ϵ��
    mass1->applyForce(force);                                                   // ����������mass1
    mass2->applyForce(-force);                                                  // -force������mass2
}