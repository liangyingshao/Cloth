#pragma once
#include "mass.h"
#include "spring.h"
#include <iostream>

class RopeSimulator// ������
{
private:
    int numOfMasses;// �ʵ����
    glm::vec3 gravitation;// ����
    float airFrictionConstant;// ����Ħ��ϵ��
    float groundRepulsionConstant;// �ذ��ų�ϵ��
    float groundFrictionConstant;// �ذ�Ħ��ϵ��
    float groundAbsorptionConstant;// �ذ�����ϵ��
    float groundHeight;// �ذ�߶�
    Mass** masses;// �ʵ�����
    Spring** springs;// ��������
    glm::vec3 ropeConnectionVel;// �������ҵ���ٶ�
    glm::vec3 ropeConnectionPos;// �������ҵ��λ��
public:
    ~RopeSimulator();
    RopeSimulator();
    RopeSimulator(
        int numOfMasses,								//1. �ʵ����
        float m,										//2. ÿ���ʵ�������ͬ
        float springConstant,							//3. ���ɵĵ���ϵ��
        float springLength,								//4. ����ԭ��
        float springFrictionConstant,					//5. ���ɵ�����ϵ��
        glm::vec3 g,							        //6. �������ٶ�
        float airFrictionConstant,						//7. ����Ħ��ϵ��
        float groundRepulsionConstant,					//8. �ذ��ų�ϵ��
        float groundFrictionConstant,					//9. �ذ�Ħ��ϵ��
        float groundAbsorptionConstant,					//10. �ذ�����ϵ��
        float groundHeight								//11. �ذ�߶� (y)
    );
    void release();
    void simulate(float dt);
    void setRopeConnectionPos(glm::vec3 p);
    void setRopeConnectionVel(glm::vec3 v);
    float getGroundHeight();
    int getNumOfMasses();
    Mass* getMass(int index);
    void operate(float dt);
    void solve();
    void resetMassesForce();
};