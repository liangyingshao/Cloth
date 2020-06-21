#pragma once
#include <glm/glm.hpp>
#include <iostream>
class Mass
{
public:
    Mass(float m=0.005);
    ~Mass();
    void applyForce(glm::vec3 force);//加力
    void init();//初始化力
    void simulate(float dt);//质点在dt时间内的运动，改变速度和位置，显式欧拉法：步长尽量小一点，步长可能是不稳定的原因
    void setPos(glm::vec3 p);//设置位置
    void setVel(glm::vec3 v);//设置速度
    void setFixed(bool b);//设置是否固定
    void setCollision(glm::vec3 v);//设置冲力
    void setNormal(glm::vec3 v);//设置法向量
    void setLastPos(glm::vec3 p);//设置前一位置
    void applyNormal(glm::vec3 v);//加上不同三角形的法向量

    glm::vec3 getPos();//获取位置
    glm::vec3 getVel();//获取速度
    float getM();//获取质量
    bool getFixed();//获取是否固定
    glm::vec3 getCollision();//获取冲力
    glm::vec3 getNormal();//获取法向量
    int getIndex();
    glm::vec3 getLastPos();
    glm::vec3 getForce();
    void setLastForce(glm::vec3 f);
protected:
private:
    //质量,应在构造函数中初始化
    float m;
    //坐标
    glm::vec3 pos;
    //速度
    glm::vec3 vel;
    //作用在这个质点上的力
    glm::vec3 force;
    //该质点是否固定
    bool fixed;
    //质点与球发生碰撞后受到的冲力
    glm::vec3 collision;
    //法向量
    glm::vec3 normal;
    //cube的哪个面
    int faceIndex=0;
    glm::vec3 lastPos;
    glm::vec3 lastForce;
    float lastTime=0;
};