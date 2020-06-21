#pragma once
#include "mass.h"
#include <iostream>
class Spring
{
public:
    Spring();
    Spring(Mass* mass1, Mass* mass2,
        float springConstant, float springLength,
        float frictionConstant);//初始化所有成员变量
    ~Spring();

    void solve();//计算弹簧的两个质点所受的阻尼力和弹力
protected:
private:
    //一个弹簧由两端的两个质点组成
    Mass* mass1;//质点1
    Mass* mass2;//质点2

    float springConstant;//弹性系数
    float restLength;//弹簧原长
    float frictionConstant;//阻尼系数

};