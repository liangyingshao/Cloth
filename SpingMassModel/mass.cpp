#include "mass.h"
extern float radius;
extern float cubeSize;
extern bool sphere_cube;
float collisionFactor = 0.6;
glm::vec3 cubeNormals[6] = {
    glm::vec3(0,1,0),glm::vec3(0,-1,0),//上下
    glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,-1.0f),//前后
    glm::vec3(-1,0,0),glm::vec3(1,0,0)//左右
};
float a= cubeSize/2, b= cubeSize/2, c= cubeSize/2;

Mass::Mass(float m)
{
    this->m = m;
    this->fixed = false;
}

Mass::~Mass()
{
}

void Mass::applyForce(glm::vec3 f)
{
    this->force += f;
}

void Mass::init()
{
    lastForce = force;
    force.x = 0;
    force.y = 0;
    force.z = 0;
}

void Mass::simulate(float dt)
{
    if (this->fixed)
        return;
    glm::vec3 oldVel = vel;
    //Velocity Verlet
    vel = vel + (lastForce + force) / (m*2) * lastTime;//首先更新当前速度
    glm::vec3 newPos = pos+vel*dt+ 0.5f * (force / m) * dt * dt;
    glm::vec3 newVel = vel+(force / m) * dt;//这是当前时间加上dt后的预测速度
    //显式欧拉
    /*glm::vec3 newVel = vel + (force / m) * dt;
    glm::vec3 newPos = pos + newVel * dt;*/
    
    if (glm::length(newPos) < radius&& sphere_cube)                          //判断是否与球碰撞
    {
        float high = dt;
        float low = 0;
        float time;
        glm::vec3 currentPos;
        for (int i=0; abs(glm::length(currentPos) - radius)>0.01&&i<100;i++)    //折半查找法求出碰撞的时间点
            //i的限制是为了避免死循环，一般找了100次时间上已经足够接近碰撞点了
        {
            //测试用：float len = glm::length(currentPos);
            time = (high + low) / 2.0f;
            //newVel = oldVel + (force / m) / time;
            currentPos = pos + vel * time + 0.5f * (force / m) * time * time;//pos + newVel * dt;//显式欧拉或velocity verlet
            if (glm::length(currentPos) < radius)
                high = (high + low) / 2.0f;
            else if(glm::length(currentPos) > radius)
                low = (high + low) / 2.0f;
        }
        lastTime = time;
        //vel += (force / m) * time;
        pos = glm::normalize(pos+vel*time) * radius;                                  //将质点移动到球的表面
        vel = vel - 2.0f * (glm::dot(vel+ (force / m) * time, glm::normalize(newPos))) * glm::normalize(newPos);   //碰撞后，速度镜像反射
        vel *= collisionFactor;//碰撞速度损失
        collision = glm::dot((vel - oldVel), glm::normalize(pos)) * glm::normalize(pos) * m;//有发现方向上的速度变化才会有碰撞力
    }
    else if (newPos.x<a&& newPos.x>-a&& newPos.y<b&& newPos.y>-b&& newPos.z<c&& newPos.z>-c&&!sphere_cube)     //与cube相撞
    {
        float timeArr[6];
        if (abs(b - pos.y) < 0.001)
        {
            pos.y = b;
            timeArr[0] = 0;
            lastTime = 0;
            faceIndex = 0;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[0] = (b - pos.y) / newVel.y;
        if (abs(-b - pos.y) < 0.001)
        {
            pos.y = -b;
            timeArr[1] = 0;
            lastTime = 0;
            faceIndex = 1;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[1] = (-b - pos.y) / newVel.y;
        if (abs(c - pos.z) < 0.001)
        {
            pos.z = c;
            timeArr[2] = 0;
            lastTime = 0;
            faceIndex = 2;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[2] = (c - pos.z) / newVel.z;
        if (abs(-c - pos.z) < 0.001)
        {
            pos.z = -c;
            timeArr[3] = 0;
            lastTime = 0;
            faceIndex = 3;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[3] = (-c - pos.z) / newVel.z;
        if (abs(-a - pos.x) < 0.001)
        {
            pos.x = -a;
            timeArr[4] = 0;
            lastTime = 0;
            faceIndex = 4;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[4] = (-a - pos.x) / newVel.x;
        if (abs(a - pos.x) < 0.001)
        {
            pos.x = a;
            timeArr[5] = 0;
            lastTime = 0;
            faceIndex = 5;
            vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[faceIndex])) * glm::normalize(cubeNormals[faceIndex]));
            collision = glm::dot((vel - oldVel), cubeNormals[faceIndex]) * cubeNormals[faceIndex] * m;
            return;
        }
        else
            timeArr[5] = (a - pos.x) / newVel.x;

        int index = 0;
        for (int i = 0; i < 6; i++)
        {
            if (timeArr[i] < 0||timeArr[i]>dt)continue;
            float currentX = timeArr[i] * newVel.x + pos.x;
            float currentY = timeArr[i] * newVel.y + pos.y;
            float currentZ = timeArr[i] * newVel.z + pos.z;
            if (currentX <= a && currentX >= -a && currentY <= b && currentY >= -b && currentZ <= c && currentZ >= -c
                &&timeArr[index]>timeArr[i])
            {
                index = i;
            }
        }
        if (timeArr[index] < 0 || timeArr[index] > dt)
        {
            lastTime = 0;
            timeArr[index] = 0;
        }
        faceIndex = index;
        pos.x = timeArr[index] * newVel.x + pos.x;
        pos.y = timeArr[index] * newVel.y + pos.y;
        pos.z = timeArr[index] * newVel.z + pos.z;
        
        //lastTime = timeArr[index];
        //pos += newVel * timeArr[index];//后面考虑移到表面
        ////vel += (force / m) * timeArr[index]; 
        vel = collisionFactor * (vel - 2.0f * (glm::dot(vel, cubeNormals[index])) * glm::normalize(cubeNormals[index]));
        collision = glm::dot((vel - oldVel), cubeNormals[index]) * cubeNormals[index] * m;
    }
    else
    {
        //显式欧拉
        /*vel += (force / m) * dt;
        pos += vel * dt;*/
        //velocity verlet
        pos = pos + vel * dt + 0.5f * (force / m) * dt * dt;
        lastTime = dt;
        //速度将在下一次更新vel = vel + (force / m) * dt;
    }
}

glm::vec3 Mass::getPos()
{
    return pos;
}

void Mass::setPos(glm::vec3 p)
{
    pos = p;
}

void Mass::setVel(glm::vec3 v)
{
    this->vel = v;
}

void Mass::setFixed(bool b)
{
    this->fixed = b;
}

void Mass::setCollision(glm::vec3 v)
{
    this->collision = v;
}

void Mass::setNormal(glm::vec3 v)
{
    this->normal = v;
}

void Mass::setLastPos(glm::vec3 p)
{
    this->lastPos = p;
}

void Mass::setLastForce(glm::vec3 f)
{
    this->lastForce = f;
}

void Mass::applyNormal(glm::vec3 v)
{
    this->normal += v;
}

glm::vec3 Mass::getVel()
{
    return this->vel;
}


float Mass::getM()
{
    return this->m;
}

bool Mass::getFixed()
{
    return this->fixed;
}

glm::vec3 Mass::getCollision()
{
    return this->collision;
}

glm::vec3 Mass::getNormal()
{
    return this->normal;
}

int Mass::getIndex()
{
    return this->faceIndex;
}

glm::vec3 Mass::getLastPos()
{
    return this->lastPos;
}

glm::vec3 Mass::getForce()
{
    return this->force;
}