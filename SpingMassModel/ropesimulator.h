#pragma once
#include "mass.h"
#include "spring.h"
#include <iostream>

class RopeSimulator// 绳索类
{
private:
    int numOfMasses;// 质点个数
    glm::vec3 gravitation;// 重力
    float airFrictionConstant;// 空气摩擦系数
    float groundRepulsionConstant;// 地板排斥系数
    float groundFrictionConstant;// 地板摩擦系数
    float groundAbsorptionConstant;// 地板吸收系数
    float groundHeight;// 地板高度
    Mass** masses;// 质点数组
    Spring** springs;// 弹簧数组
    glm::vec3 ropeConnectionVel;// 绳子悬挂点的速度
    glm::vec3 ropeConnectionPos;// 绳子悬挂点的位置
public:
    ~RopeSimulator();
    RopeSimulator();
    RopeSimulator(
        int numOfMasses,								//1. 质点个数
        float m,										//2. 每个质点质量相同
        float springConstant,							//3. 弹簧的弹性系数
        float springLength,								//4. 弹簧原长
        float springFrictionConstant,					//5. 弹簧的阻尼系数
        glm::vec3 g,							        //6. 重力加速度
        float airFrictionConstant,						//7. 空气摩擦系数
        float groundRepulsionConstant,					//8. 地板排斥系数
        float groundFrictionConstant,					//9. 地板摩擦系数
        float groundAbsorptionConstant,					//10. 地板吸收系数
        float groundHeight								//11. 地板高度 (y)
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