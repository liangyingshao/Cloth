#include "ropesimulator.h"

RopeSimulator::RopeSimulator()
{
    //ctor
}

RopeSimulator::~RopeSimulator()
{
    //dtor
    release();
}

RopeSimulator::RopeSimulator(
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
)
{
    this->numOfMasses = numOfMasses;
    this->gravitation = g;
    this->airFrictionConstant = airFrictionConstant;
    this->groundFrictionConstant = groundFrictionConstant;
    this->groundRepulsionConstant = groundRepulsionConstant;
    this->groundAbsorptionConstant = groundAbsorptionConstant;
    this->groundHeight = groundHeight;

    this->masses = new Mass * [numOfMasses];

    for (int count = 0; count < numOfMasses; ++count)		    // �����������ʵ㲢��ʼ����
    {
        masses[count] = new Mass(m);
        masses[count]->init();
    }

    for (int index = 0; index < numOfMasses; ++index)			// ��ʼ��������ÿһ���ʵ��λ�ã�ÿ�����ʵ����һ������ԭ��(x)
    {
        masses[index]->setPos(glm::vec3(0, -index * springLength, 0));
    }

    springs = new Spring * [numOfMasses - 1];
    for (int index = 0; index < numOfMasses - 1; ++index)			// ����numOfMasses - 1������
    {
        springs[index] = new Spring(masses[index], masses[index + 1],
            springConstant, springLength, springFrictionConstant);
    }
}

void RopeSimulator::release()
{
    for (int count = 0; count < numOfMasses; ++count)       // delete�����ʵ��ָ������
    {
        delete(masses[count]);
        masses[count] = NULL;
    }

    delete(masses);
    masses = NULL;

    for (int index = 0; index < numOfMasses - 1; ++index)   // delete���е��ɺ�ָ������
    {
        delete(springs[index]);
        springs[index] = NULL;
    }

    delete(springs);
    springs = NULL;
}

//����ÿһ���ʵ��λ�ú��ٶ�
void RopeSimulator::simulate(float dt)
{
    for (int count = 0; count < numOfMasses; ++count)		// ����ÿһ���ʵ㾭��dt���v��pos
    {
        masses[count]->simulate(dt);
        // std::cout<<"mass "<<count<<" pos: "<<masses[count]->getVel().x<<masses[count]->getVel().y<<masses[count]->getVel().z<<std::endl;
    }

    ropeConnectionPos += ropeConnectionVel * dt;	// ����ropeConnectionVel

    if (ropeConnectionPos.y < groundHeight)			// ������أ���Vy=0�����ҵ��y���ڵذ�ĸ߶�
    {
        ropeConnectionPos.y = groundHeight;
        ropeConnectionVel.y = 0;
    }
    masses[0]->setPos(ropeConnectionPos);				//��һ���ʵ�λ�������ҵ�
    masses[0]->setVel(ropeConnectionVel);				//��һ���ʵ��ٶ������ҵ���ͬ

}

void RopeSimulator::setRopeConnectionPos(glm::vec3 p)
{
    this->ropeConnectionPos = p;
}

void RopeSimulator::setRopeConnectionVel(glm::vec3 v)
{
    this->ropeConnectionVel = v;
}

float RopeSimulator::getGroundHeight()
{
    return this->groundHeight;
}

int RopeSimulator::getNumOfMasses()
{
    return this->numOfMasses;
}

Mass* RopeSimulator::getMass(int index)
{
    if (index < 0 || index >= numOfMasses)
        return NULL;
    return masses[index];
}

void RopeSimulator::operate(float dt)
{
    this->resetMassesForce();
    this->solve();
    this->simulate(dt);
}

void RopeSimulator::resetMassesForce()								// ����ÿһ���ʵ��init()
{
    for (int count = 0; count < numOfMasses; ++count)
        masses[count]->init();
}

void RopeSimulator::solve()
{
    for (int index = 0; index < numOfMasses - 1; ++index)//����ÿ�����ɵ��������͵���
    {
        springs[index]->solve();

    }

    for (int index = 0; index < numOfMasses; ++index)				//����ÿ���ʵ������
    {
        masses[index]->applyForce(gravitation * masses[index]->getM());				//��������
        masses[index]->applyForce(-masses[index]->getVel() * airFrictionConstant);	//���ӿ���Ħ����

        if (masses[index]->getPos().y < groundHeight)		//������ף��������ذ��Ħ��������ײ����������
        {
            glm::vec3 v;								    //��ʱ����

            //���ӻ���Ħ����
            //y�����ϵ��ٶȱ������ˣ���Ϊ����Ҫʩ��Ħ��������������Ч�������������ƽ�С�
            v = masses[index]->getVel();					//��ȡ�ٶ�
            v.y = 0;								        //y�����ٶȱ�Ϊ0
            masses[index]->applyForce(-v * groundFrictionConstant);		//ground friction force is applied

            //��y�����ϵ��ٶȽ���������ЧӦ��
            v = masses[index]->getVel();
            v.x = 0;								//����x��z������ٶ�
            v.z = 0;
            if (v.y < 0)							//ֻ�е����Ӻ͵��������ײ��ʱ��������ЧӦ����ʱy<0
                masses[index]->applyForce(-v * groundAbsorptionConstant);

            //�������һ������һ���ų�����
            //Vector3D(0, groundRepulsionConstant, 0)�ǵ��淨�߷��������
            //��һ�ų������ʵ�Ե���ĳ����һ���󣬳������(groundHeight - masses[a]->pos.y)����
            glm::vec3 force = glm::vec3(0, groundRepulsionConstant, 0) *
                (groundHeight - masses[index]->getPos().y);
            masses[index]->applyForce(force);
        }

    }
}