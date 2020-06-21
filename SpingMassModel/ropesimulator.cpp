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

    for (int count = 0; count < numOfMasses; ++count)		    // 生成绳索中质点并初始化力
    {
        masses[count] = new Mass(m);
        masses[count]->init();
    }

    for (int index = 0; index < numOfMasses; ++index)			// 初始化绳索中每一个质点的位置，每两个质点相距一个弹簧原长(x)
    {
        masses[index]->setPos(glm::vec3(0, -index * springLength, 0));
    }

    springs = new Spring * [numOfMasses - 1];
    for (int index = 0; index < numOfMasses - 1; ++index)			// 生成numOfMasses - 1个弹簧
    {
        springs[index] = new Spring(masses[index], masses[index + 1],
            springConstant, springLength, springFrictionConstant);
    }
}

void RopeSimulator::release()
{
    for (int count = 0; count < numOfMasses; ++count)       // delete所有质点和指针数组
    {
        delete(masses[count]);
        masses[count] = NULL;
    }

    delete(masses);
    masses = NULL;

    for (int index = 0; index < numOfMasses - 1; ++index)   // delete所有弹簧和指针数组
    {
        delete(springs[index]);
        springs[index] = NULL;
    }

    delete(springs);
    springs = NULL;
}

//计算每一个质点的位置和速度
void RopeSimulator::simulate(float dt)
{
    for (int count = 0; count < numOfMasses; ++count)		// 计算每一个质点经过dt后的v和pos
    {
        masses[count]->simulate(dt);
        // std::cout<<"mass "<<count<<" pos: "<<masses[count]->getVel().x<<masses[count]->getVel().y<<masses[count]->getVel().z<<std::endl;
    }

    ropeConnectionPos += ropeConnectionVel * dt;	// 更新ropeConnectionVel

    if (ropeConnectionPos.y < groundHeight)			// 如果触地，让Vy=0，悬挂点的y等于地板的高度
    {
        ropeConnectionPos.y = groundHeight;
        ropeConnectionVel.y = 0;
    }
    masses[0]->setPos(ropeConnectionPos);				//第一个质点位置在悬挂点
    masses[0]->setVel(ropeConnectionVel);				//第一个质点速度与悬挂点相同

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

void RopeSimulator::resetMassesForce()								// 调用每一个质点的init()
{
    for (int count = 0; count < numOfMasses; ++count)
        masses[count]->init();
}

void RopeSimulator::solve()
{
    for (int index = 0; index < numOfMasses - 1; ++index)//计算每个弹簧的阻尼力和弹力
    {
        springs[index]->solve();

    }

    for (int index = 0; index < numOfMasses; ++index)				//计算每个质点的受力
    {
        masses[index]->applyForce(gravitation * masses[index]->getM());				//增加重力
        masses[index]->applyForce(-masses[index]->getVel() * airFrictionConstant);	//增加空气摩擦力

        if (masses[index]->getPos().y < groundHeight)		//如果触底，则添加与地板的摩擦力、碰撞力、吸收力
        {
            glm::vec3 v;								    //临时变量

            //增加滑动摩擦力
            //y方向上的速度被忽略了，因为我们要施加摩擦力来产生滑动效果。滑动与地面平行。
            v = masses[index]->getVel();					//获取速度
            v.y = 0;								        //y方向速度变为0
            masses[index]->applyForce(-v * groundFrictionConstant);		//ground friction force is applied

            //在y方向上的速度将用于吸收效应。
            v = masses[index]->getVel();
            v.x = 0;								//忽略x、z方向的速度
            v.z = 0;
            if (v.y < 0)							//只有当绳子和地面产生碰撞的时候有吸收效应，此时y<0
                masses[index]->applyForce(-v * groundAbsorptionConstant);

            //地面会像一个弹簧一样排斥物体
            //Vector3D(0, groundRepulsionConstant, 0)是地面法线方向的向量
            //这一排斥力与质点对地面的冲击力一样大，冲击力由(groundHeight - masses[a]->pos.y)衡量
            glm::vec3 force = glm::vec3(0, groundRepulsionConstant, 0) *
                (groundHeight - masses[index]->getPos().y);
            masses[index]->applyForce(force);
        }

    }
}