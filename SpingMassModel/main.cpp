#include "ropesimulator.h"
#include "cloth.h"
#include <GL/glut.h>
//#include <Eigen/Core>
#include<iostream>
using namespace std;

bool sphere_cube=false;//true-sphere,false-cube
bool line = true;
bool spot = false;
bool drawCloth = false;
cloth myCloth;
GLint viewport[4];
GLdouble PP[16];
float radius = 5.0f;
float cubeSize =7.0f;

void vec3Tofloat(glm::vec3 v, GLfloat* arr)
{
	arr[0] = v.x;
	arr[1] = v.y;
	arr[2] = v.z;
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//�����ӽ�
	//glPushMatrix();
	glTranslatef(0.0, 0, -20);//z-20
	glRotatef(15, 1, 0, 0);

	//���ù�Դ
	{
		GLfloat light_position[] = { 0.0f, 10.0f, 100.0f, 1.0f };
		GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0 };
		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_POSITION, light_position); //ָ����0�Ź�Դ��λ�ã����ĸ�ֵΪ0��ʾ�Ƿ����
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //��η���
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //������
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//���淴��

		glEnable(GL_LIGHT0); //ʹ�õ�0�Ź���
		glEnable(GL_LIGHTING); //�ں������Ⱦ��ʹ�ù���
	}
	//���ò���
	{
		//��
		//GLfloat mat_ambient[] = { 0.192250f, 0.192250, 0.192250, 1.0f };//��η���
		//GLfloat mat_diffuse[] = { 0.507540f, 0.507540f, 0.507540f, 1.0f };//������
		//GLfloat mat_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };//���淴��
		//GLfloat mat_shininess = 51.200001f;

		//����
		//GLfloat mat_ambient[] = { 0.250000, 0.207250, 0.207250, 0.922000 };//��η���
		//GLfloat mat_diffuse[] = { 1.0, 0.829, 0.829, 0.922000 };//������
		//GLfloat mat_specular[] = { 0.296648, 0.296648, 0.296648, 0.922000 };//���淴��
		//GLfloat mat_shininess = 11.264000;

		//����ʯ
		GLfloat mat_ambient[] = { 0.053750, 0.05, 0.066250, 0.82 };//��η���
		GLfloat mat_diffuse[] = { 0.182750, 0.17, 0.225250, 0.82 };//������
		GLfloat mat_specular[] = { 0.332741, 0.328634, 0.346435, 0.82 };//���淴��
		GLfloat mat_shininess = 38.400002;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess); 
	}
	if (sphere_cube)
	{
		static GLUquadricObj* sphere = gluNewQuadric();
		gluSphere(sphere, 0.99*radius, 48, 48);//Ϊ�˲�������Խ�¼�
	}
	else
	{
		glutSolidCube(cubeSize*0.97);
	}

	//���ò���
	{
		//�����
		//GLfloat mat_ambient2[] = { 0.247250, 0.199500, 0.074500, 1.0 };//��η���
		//GLfloat mat_diffuse2[] = { 0.751640, 0.606480, 0.226480, 1.0 };//������
		//GLfloat mat_specular[] = { 0.628281, 0.606480, 0.226480, 1.0 };//���淴��
		//GLfloat mat_shininess = 51.200001;

		//������
		GLfloat mat_ambient1[] = { 0.55, 0.06, 0.5, 1.0 };//��η���
		GLfloat mat_diffuse1[] = { 0.43, 0.47, 0.54, 1.0 };//������
		//GLfloat mat_specular[] = { 0.33, 0.33, 0.52, 1.0 };//���淴��
		//GLfloat mat_shininess = 22;

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse1);
	}
	if (drawCloth)
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < MASS_NUM - 1; i++)
		{
			for (int j = 0; j < MASS_NUM - 1; j++)
			{
				GLfloat normalArr[3];
				vec3Tofloat(myCloth.getMass(i, j)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
				vec3Tofloat(myCloth.getMass(i, j + 1)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i, j + 1)->getPos().x, myCloth.getMass(i, j + 1)->getPos().y, myCloth.getMass(i, j + 1)->getPos().z);
				vec3Tofloat(myCloth.getMass(i + 1, j)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i + 1, j)->getPos().x, myCloth.getMass(i + 1, j)->getPos().y, myCloth.getMass(i + 1, j)->getPos().z);

				vec3Tofloat(myCloth.getMass(i, j + 1)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i, j + 1)->getPos().x, myCloth.getMass(i, j + 1)->getPos().y, myCloth.getMass(i, j + 1)->getPos().z);
				vec3Tofloat(myCloth.getMass(i + 1, j)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i + 1, j)->getPos().x, myCloth.getMass(i + 1, j)->getPos().y, myCloth.getMass(i + 1, j)->getPos().z);
				vec3Tofloat(myCloth.getMass(i + 1, j + 1)->getNormal(), normalArr);
				glNormal3fv(normalArr);
				glVertex3f(myCloth.getMass(i + 1, j + 1)->getPos().x, myCloth.getMass(i + 1, j + 1)->getPos().y, myCloth.getMass(i + 1, j + 1)->getPos().z);
			}
		}
		glEnd();
	}
	glDisable(GL_LIGHTING);

	if (line)
	{
		glColor3f(0, 1.0, 0.0);
		glBegin(GL_LINES);
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				if (j != MASS_NUM - 1)
				{
					//����
					glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
					glVertex3f(myCloth.getMass(i, j + 1)->getPos().x, myCloth.getMass(i, j + 1)->getPos().y, myCloth.getMass(i, j + 1)->getPos().z);
				}

				if (i != MASS_NUM - 1)
				{
					//����
					glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
					glVertex3f(myCloth.getMass(i + 1, j)->getPos().x, myCloth.getMass(i + 1, j)->getPos().y, myCloth.getMass(i + 1, j)->getPos().z);
					//������
					//������ǵ�һ�У��Ϳ���������
					if (j != 0)
					{
						glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
						glVertex3f(myCloth.getMass(i + 1, j - 1)->getPos().x, myCloth.getMass(i + 1, j - 1)->getPos().y, myCloth.getMass(i + 1, j - 1)->getPos().z);
					}
					//����������һ�У��Ϳ���������
					if (j != MASS_NUM - 1)
					{
						glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
						glVertex3f(myCloth.getMass(i + 1, j + 1)->getPos().x, myCloth.getMass(i + 1, j + 1)->getPos().y, myCloth.getMass(i + 1, j + 1)->getPos().z);
					}
				}
				if (i < MASS_NUM - 2)
				{
					glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
					glVertex3f(myCloth.getMass(i + 2, j)->getPos().x, myCloth.getMass(i + 2, j)->getPos().y, myCloth.getMass(i + 2, j)->getPos().z);
				}
				if (j < MASS_NUM - 2)
				{
					glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
					glVertex3f(myCloth.getMass(i, j + 2)->getPos().x, myCloth.getMass(i, j + 2)->getPos().y, myCloth.getMass(i, j + 2)->getPos().z);
				}
			}
		}
		glEnd();
	}

	if(spot)
	{ 
		glPointSize(3);
		glColor3f(1.0, 0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				glVertex3f(myCloth.getMass(i, j)->getPos().x, myCloth.getMass(i, j)->getPos().y, myCloth.getMass(i, j)->getPos().z);
			}
		}
		glEnd();
		glPointSize(1);
	}

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.f, 100.0f);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, PP);

	glMatrixMode(GL_MODELVIEW);
}

float time = 0; int temp = 0;
void idlefunc()
{
	//�ı䶯������
	float newTime = (float)glutGet(GLUT_ELAPSED_TIME);
	myCloth.operate((newTime - time) / 1000.0 > 0.005 ? 0.005 : (newTime - time) / 1000.0);	//0.1��Ϊ�˱�����֮֡�����������²��ȶ�
	time = newTime;
	
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		for (int i = 0; i < MASS_NUM; i++)
		{
			for (int j = 0; j < MASS_NUM; j++)
			{
				myCloth.getMass(i,j)->setPos(glm::vec3((i * 0.2 - 5.0), 9, float(j) * 0.2 - 5.0f));
				myCloth.getMass(i, j)->setVel(glm::vec3(0, 0, 0));
				if (i == 0 && j == 0)
					myCloth.getMass(i, j)->setFixed(true);
				else if (j == 0 && i == MASS_NUM - 1)
					myCloth.getMass(i, j)->setFixed(true);
				else if (i == MASS_NUM - 1 && j == MASS_NUM - 1)
					myCloth.getMass(i, j)->setFixed(true);
				else if (i == 0 && j == MASS_NUM - 1)
					myCloth.getMass(i, j)->setFixed(true);
			}
		}
		break;
	case 't':
		sphere_cube = !sphere_cube;
		break;
	case 'l':
		line = !line; break;
	case 's':
		spot=!spot; break;
	case 'c':
		drawCloth = !drawCloth; break;
	}
}

void SpecialKey(GLint key, GLint x, GLint y)
{
	if (key == GLUT_KEY_UP)
	{
		myCloth.getMass(0, 0)->setFixed(false);
	}
	else if (key == GLUT_KEY_LEFT)
	{
		myCloth.getMass(0, MASS_NUM - 1)->setFixed(false);
	}
	else if (key == GLUT_KEY_DOWN)
	{
		myCloth.getMass(MASS_NUM - 1, 0)->setFixed(false);
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		myCloth.getMass(MASS_NUM - 1, MASS_NUM - 1)->setFixed(false);
	}
}

int main(int argc, char** argv)
{
	//glm::vec3 pos(0.5, 0.8, 0);
	//rope->setRopeConnectionPos(pos);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//˫����
	//glutDisplayFunc(display)ע��ص�����ʱ��Ҫ��������glutSwapBuffers()��������������ָ�롣
	//glutIdleFuncע��һ������ʱ���Ʋ��������� ע�����������ٵ���display������
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Mass-Spring Model");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idlefunc);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(&SpecialKey);
	glutMainLoop();
	return 0;
}