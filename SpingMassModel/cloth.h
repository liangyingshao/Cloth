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
		//��ʼ������
		gravitation = glm::vec3(0, -9.8, 0);

		//��ʼ���ʵ�λ�á�������
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

		//���ɵ���
		springs = new Spring*** [MASS_NUM];
		float length = 0.2f;
		for (int i = 0; i < MASS_NUM; i++)
		{
			springs[i] = new Spring * *[MASS_NUM];
			for (int j = 0; j < MASS_NUM; j++)
			{
				springs[i][j] = new Spring* [6];

				//���򵯻�
				if (j != MASS_NUM - 1)
				{
					springs[i][j][0] = new Spring(masses[i][j], masses[i][j + 1], 15.0f, length, 0.01f);
				}
				else
				{
					springs[i][j][0] = NULL;
				}

				//���򵯻�
				if (i != MASS_NUM - 1)
				{
					springs[i][j][1] = new Spring(masses[i][j], masses[i + 1][j], 15.0f, length, 0.01f);

					//���浯��
					//������ǵ�һ�У��Ϳ���������
					if (j != 0)
					{
						springs[i][j][2] = new Spring(masses[i][j], masses[i + 1][j - 1], 15.0f, sqrt(length* length*2), 0.01f);
					}
					else
					{
						springs[i][j][2] = NULL;
					}
					//����������һ�У��Ϳ���������
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

				//���Ե���
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
	//����ÿ���ʵ������
	void solve(float dt)
	{
		//��ÿ�����ɣ����㵯���ڲ���������������������
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

		//��ÿ���ʵ㣬��������:������
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j]->applyForce(gravitation * masses[i][j]->getM());
				//masses[i][j]->applyForce(glm::vec3(0, 0, 0.08));//����,����Ļ�ⴵ�ķ�
				
				//��ײ�������塢��
				
				if (sphere_cube)		//���������ײ��Ӧ�ü���֧������Ħ������������������ײ��
				{
					//float check = glm::length(masses[i][j]->getPos());
					if (abs(glm::length(masses[i][j]->getPos()) - radius) < 5e-3)
					{
						glm::vec3 N = masses[i][j]->getM() * abs(glm::dot(gravitation, glm::normalize(masses[i][j]->getPos())))
							* glm::normalize(masses[i][j]->getPos());
						masses[i][j]->applyForce(N);					//֧����
						glm::vec3 f = glm::normalize(masses[i][j]->getM() * gravitation - N)
							* sphereFriction * glm::length(N);
						masses[i][j]->applyForce(f);					//Ħ����
						masses[i][j]->applyForce(masses[i][j]->getCollision());	//����
						masses[i][j]->setCollision(glm::vec3(0, 0, 0));
					}
				}
				else//�����壬֧������Ħ������������������ײ��
				{
					if (masses[i][j]->getPos().x<=a && masses[i][j]->getPos().x>=-a
						&& masses[i][j]->getPos().y<=b && masses[i][j]->getPos().y>=-b
						&& masses[i][j]->getPos().z<=c && masses[i][j]->getPos().z>=-c)
					{
						//int checkIndex = masses[i][j]->getIndex();
						glm::vec3 N = abs(glm::dot(masses[i][j]->getM()*gravitation, cubeNormals[masses[i][j]->getIndex()]))*
						  cubeNormals[masses[i][j]->getIndex()];
						masses[i][j]->applyForce(N);					//֧����
						glm::vec3 f = glm::normalize(masses[i][j]->getM() * gravitation - N)
						  * sphereFriction * glm::length(N);
						masses[i][j]->applyForce(f);					//Ħ����
						masses[i][j]->applyForce(masses[i][j]->getCollision());	//����
						masses[i][j]->setCollision(glm::vec3(0, 0, 0));
					}
				}
			
				//����һ���ʵ�������������
				//����ǵ�һ������λ�ã���ô��ÿ���ʵ��ʼλ�õ�ǰһλ�ø�ֵ
				if (fisrt_pos)
				{
					masses[i][j]->setLastPos(masses[i][j]->getPos() + masses[i][j]->getForce() / masses[i][j]->getM() * dt * dt);
				}
			}
		}
		fisrt_pos = false;
	}
	//���㾭��delta t��ÿ���ʵ��λ��
	void simulate(float t)
	{
		//����λ�ú�����
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				masses[i][j]->simulate(t);
				masses[i][j]->setNormal(glm::vec3(0, 0, 0));//����������
			}
		}
		//�����µķ�����
		for (int i = 0; i < MASS_NUM-1; i++)
		{
			for (int j = 0; j < MASS_NUM-1; j++)
			{
				glm::vec3 arr[4] = {
					masses[i][j]->getPos(),masses[i][j + 1]->getPos(),
					masses[i + 1][j]->getPos(),masses[i + 1][j + 1]->getPos()
				};
				glm::vec3 currentNormal = glm::cross(arr[2] - arr[0], arr[1] - arr[0]);//�������εķ�����
				masses[i][j]->applyNormal(currentNormal);
				masses[i][j+1]->applyNormal(currentNormal);
				masses[i+1][j]->applyNormal(currentNormal);

				currentNormal = glm::cross(arr[1] - arr[3], arr[2] - arr[3]);//�������εķ�����
				masses[i+1][j+1]->applyNormal(currentNormal);
				masses[i][j + 1]->applyNormal(currentNormal);
				masses[i + 1][j]->applyNormal(currentNormal);
			}
		}
		//��������һ��
		for (int i = 0; i < MASS_NUM - 1; i++)
		{
			for (int j = 0; j < MASS_NUM - 1; j++)
			{
				glm::vec3 temp=masses[i][j]->getNormal();
				masses[i][j]->setNormal(glm::normalize(temp));
			}
		}
	}
	//����ÿ���ʵ������
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
	//������������������������λ���ٶ�
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