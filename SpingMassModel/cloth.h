#pragma once
#include "mass.h"
#include "spring.h"
#include <iostream>
extern float radius;
extern bool sphere_cube;
extern float a, b, c;
extern glm::vec3 cubeNormals[6];
float sphereFriction = 0.1;
bool fisrt_pos = true;

#define MASS_NUM 50
class cloth
{
public:
	cloth()
	{
		//初始化参数
		gravitation = glm::vec3(0, -9.8, 0);

		//初始化质点位置、法向量
		masses = new Mass * *[MASS_NUM];
		for (int i = 0; i < MASS_NUM; i++)
		{
			masses[i] = new Mass * [MASS_NUM];
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j] = new Mass();
				masses[i][j]->setPos(glm::vec3((i * 0.2 - 5.0), 9, float(j) * 0.2 - 5.0f));
				masses[i][j]->setNormal(glm::vec3(0,1,0));
				/*if (i == 0 && j == 0)
					masses[i][j]->setFixed(true);
				else if(j==0&&i==MASS_NUM-1)
					masses[i][j]->setFixed(true);
				else if(i == MASS_NUM - 1 && j == MASS_NUM - 1)
					masses[i][j]->setFixed(true);
				else if (i == 0 && j == MASS_NUM - 1)
					masses[i][j]->setFixed(true);*/
			}
		}

		//生成弹簧
		springs = new Spring*** [MASS_NUM];
		float length = 0.2f;
		for (int i = 0; i < MASS_NUM; i++)
		{
			springs[i] = new Spring * *[MASS_NUM];
			for (int j = 0; j < MASS_NUM; j++)
			{
				springs[i][j] = new Spring* [6];

				//竖向弹簧
				if (j != MASS_NUM - 1)
				{
					springs[i][j][0] = new Spring(masses[i][j], masses[i][j + 1], 15.0f, length, 0.01f);
				}
				else
				{
					springs[i][j][0] = NULL;
				}

				//横向弹簧
				if (i != MASS_NUM - 1)
				{
					springs[i][j][1] = new Spring(masses[i][j], masses[i + 1][j], 15.0f, length, 0.01f);

					//交叉弹簧
					//如果不是第一列，就可以连左下
					if (j != 0)
					{
						springs[i][j][2] = new Spring(masses[i][j], masses[i + 1][j - 1], 15.0f, sqrt(length* length*2), 0.01f);
					}
					else
					{
						springs[i][j][2] = NULL;
					}
					//如果不是最后一列，就可以连右下
					if (j != MASS_NUM - 1) 
					{
						springs[i][j][3] = new Spring(masses[i][j], masses[i + 1][j + 1], 15.0f, sqrt(length * length * 2), 0.01f);
					}
					else
					{
						springs[i][j][3] = NULL;
					}
				}
				else
				{
					springs[i][j][1] = NULL;
					springs[i][j][2] = NULL;
					springs[i][j][3] = NULL;
				}

				//柔性弹簧
				if (i < MASS_NUM - 2)
				{
					springs[i][j][4] = new Spring(masses[i][j], masses[i + 2][j], 15.0f, length * 2, 0.01f);
				}
				else
				{
					springs[i][j][4] = NULL;
				}
				if (j < MASS_NUM - 2)
				{
					springs[i][j][5] = new Spring(masses[i][j], masses[i][j + 2], 15.0f, length * 2, 0.01f);
				}
				else
				{
					springs[i][j][5] = NULL;
				}
			}
		}
	}
	~cloth()
	{
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				delete masses[i][j];
				masses[i][j] = NULL;
			}
			delete masses[i];
			masses[i] = NULL;
		}
		delete masses;
		masses = NULL;

		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					delete springs[i][j][k];
					springs[i][j][k] = NULL;
				}
				delete springs[i][j];
				springs[i][j] = NULL;
			}
			delete springs[i];
			springs[i] = NULL;
		}
		delete springs;
		springs = NULL;
	}
	//计算每个质点的受力
	void solve(float dt)
	{
		//对每个弹簧，计算弹簧内部的受力：弹力、阻尼力
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					if (springs[i][j][k] != NULL)
					{
						springs[i][j][k]->solve();
					}
				}
			}
		}

		//对每个质点，计算外力:重力等
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j]->applyForce(gravitation * masses[i][j]->getM());
				//masses[i][j]->applyForce(glm::vec3(0, 0, 0.08));//风力,向屏幕外吹的风
				
				//碰撞：立方体、球
				
				if (sphere_cube)		//如果与球碰撞，应该加上支持力、摩擦力、（冲量定理）碰撞力
				{
					//float check = glm::length(masses[i][j]->getPos());
					if (abs(glm::length(masses[i][j]->getPos()) - radius) < 5e-3)
					{
						glm::vec3 N = masses[i][j]->getM() * abs(glm::dot(gravitation, glm::normalize(masses[i][j]->getPos())))
							* glm::normalize(masses[i][j]->getPos());
						masses[i][j]->applyForce(N);					//支持力
						glm::vec3 f = glm::normalize(masses[i][j]->getM() * gravitation - N)
							* sphereFriction * glm::length(N);
						masses[i][j]->applyForce(f);					//摩擦力
						masses[i][j]->applyForce(masses[i][j]->getCollision());	//冲力
						masses[i][j]->setCollision(glm::vec3(0, 0, 0));
					}
				}
				else//立方体，支持力、摩擦力、（冲量定理）碰撞力
				{
					if (masses[i][j]->getPos().x<=a && masses[i][j]->getPos().x>=-a
						&& masses[i][j]->getPos().y<=b && masses[i][j]->getPos().y>=-b
						&& masses[i][j]->getPos().z<=c && masses[i][j]->getPos().z>=-c)
					{
						//int checkIndex = masses[i][j]->getIndex();
						glm::vec3 N = abs(glm::dot(masses[i][j]->getM()*gravitation, cubeNormals[masses[i][j]->getIndex()]))*
						  cubeNormals[masses[i][j]->getIndex()];
						masses[i][j]->applyForce(N);					//支持力
						glm::vec3 f = glm::normalize(masses[i][j]->getM() * gravitation - N)
						  * sphereFriction * glm::length(N);
						masses[i][j]->applyForce(f);					//摩擦力
						masses[i][j]->applyForce(masses[i][j]->getCollision());	//冲力
						masses[i][j]->setCollision(glm::vec3(0, 0, 0));
					}
				}
			
				//至此一个质点的受力计算完毕
				//如果是第一个计算位置，那么给每个质点初始位置的前一位置赋值
				if (fisrt_pos)
				{
					masses[i][j]->setLastPos(masses[i][j]->getPos() + masses[i][j]->getForce() / masses[i][j]->getM() * dt * dt);
				}
			}
		}
		fisrt_pos = false;
	}
	//计算经过delta t后每个质点的位置
	void simulate(float t)
	{
		//计算位置和受力
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j]->simulate(t);
				masses[i][j]->setNormal(glm::vec3(0, 0, 0));//法向量置零
			}
		}
		//计算新的法向量
		for (int i = 0; i < MASS_NUM-1; i++)
		{
			for (int j = 0; j < MASS_NUM-1; j++)
			{
				glm::vec3 arr[4] = {
					masses[i][j]->getPos(),masses[i][j + 1]->getPos(),
					masses[i + 1][j]->getPos(),masses[i + 1][j + 1]->getPos()
				};
				glm::vec3 currentNormal = glm::cross(arr[2] - arr[0], arr[1] - arr[0]);//上三角形的法向量
				masses[i][j]->applyNormal(currentNormal);
				masses[i][j+1]->applyNormal(currentNormal);
				masses[i+1][j]->applyNormal(currentNormal);

				currentNormal = glm::cross(arr[1] - arr[3], arr[2] - arr[3]);//下三角形的法向量
				masses[i+1][j+1]->applyNormal(currentNormal);
				masses[i][j + 1]->applyNormal(currentNormal);
				masses[i + 1][j]->applyNormal(currentNormal);
			}
		}
		//法向量归一化
		for (int i = 0; i < MASS_NUM - 1; i++)
		{
			for (int j = 0; j < MASS_NUM - 1; j++)
			{
				glm::vec3 temp=masses[i][j]->getNormal();
				masses[i][j]->setNormal(glm::normalize(temp));
			}
		}
	}
	//重置每个质点的受力
	void resetMassesForce()
	{
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j]->init();
			}
		}
	}
	//重置受力、计算受力、计算位置速度
	void operate(float t)
	{
		resetMassesForce();
		solve(t);
		simulate(t);
	}
	Mass* getMass(int row, int column);
private:
	Mass*** masses;
	Spring**** springs;
	glm::vec3 gravitation;
};

Mass* cloth::getMass(int row, int column)
{
	return this->masses[row][column];
}