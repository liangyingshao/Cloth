#pragma once
#include <glm/glm.hpp>
#include <iostream>
class Mass
{
public:
    Mass(float m=0.005);
    ~Mass();
    void applyForce(glm::vec3 force);//����
    void init();//��ʼ����
    void simulate(float dt);//�ʵ���dtʱ���ڵ��˶����ı��ٶȺ�λ�ã���ʽŷ��������������Сһ�㣬���������ǲ��ȶ���ԭ��
    void setPos(glm::vec3 p);//����λ��
    void setVel(glm::vec3 v);//�����ٶ�
    void setFixed(bool b);//�����Ƿ�̶�
    void setCollision(glm::vec3 v);//���ó���
    void setNormal(glm::vec3 v);//���÷�����
    void setLastPos(glm::vec3 p);//����ǰһλ��
    void applyNormal(glm::vec3 v);//���ϲ�ͬ�����εķ�����

    glm::vec3 getPos();//��ȡλ��
    glm::vec3 getVel();//��ȡ�ٶ�
    float getM();//��ȡ����
    bool getFixed();//��ȡ�Ƿ�̶�
    glm::vec3 getCollision();//��ȡ����
    glm::vec3 getNormal();//��ȡ������
    int getIndex();
    glm::vec3 getLastPos();
    glm::vec3 getForce();
    void setLastForce(glm::vec3 f);
protected:
private:
    //����,Ӧ�ڹ��캯���г�ʼ��
    float m;
    //����
    glm::vec3 pos;
    //�ٶ�
    glm::vec3 vel;
    //����������ʵ��ϵ���
    glm::vec3 force;
    //���ʵ��Ƿ�̶�
    bool fixed;
    //�ʵ���������ײ���ܵ��ĳ���
    glm::vec3 collision;
    //������
    glm::vec3 normal;
    //cube���ĸ���
    int faceIndex=0;
    glm::vec3 lastPos;
    glm::vec3 lastForce;
    float lastTime=0;
};