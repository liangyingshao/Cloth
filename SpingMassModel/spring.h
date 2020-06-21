#pragma once
#include "mass.h"
#include <iostream>
class Spring
{
public:
    Spring();
    Spring(Mass* mass1, Mass* mass2,
        float springConstant, float springLength,
        float frictionConstant);//��ʼ�����г�Ա����
    ~Spring();

    void solve();//���㵯�ɵ������ʵ����ܵ��������͵���
protected:
private:
    //һ�����������˵������ʵ����
    Mass* mass1;//�ʵ�1
    Mass* mass2;//�ʵ�2

    float springConstant;//����ϵ��
    float restLength;//����ԭ��
    float frictionConstant;//����ϵ��

};