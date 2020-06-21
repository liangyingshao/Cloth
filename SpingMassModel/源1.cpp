#include "GL/glut.h"
#include "math.h"


#define blueBodyBlackArm 1
//#define 




//裤子 上衣 长裙 短裙
#define pants 0
#define clothes1 1
#define clothes2 2
#define clothes3 3
#define longSkirt 4
#define shortSkirt 5






int colorMode = 0;
int style = 0; //款式变换  由于需要用到菜单功能，所以款式变换放在菜单里面了(颜色变化就用右键了)


void cloth1(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 2, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glVertex3f(25.0, 120.0, 0.0);
	glVertex3f(15.0, 130.0, 0.0);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(75.0, 120.0, 0.0);
	glVertex3f(85.0, 130.0, 0.0);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(75.0, 120.0, 0.0);
	glVertex3f(85.0, 130.0, 0.0);
	glEnd();




	glColor3f(0, 2, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(35.0, 60.0, 0.0);
	glVertex3f(65.0, 60.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glutSwapBuffers();
}


void cloth2(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glVertex3f(25.0, 120.0, 0.0);
	glVertex3f(15.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(75.0, 120.0, 0.0);
	glVertex3f(85.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 60.0, 0.0);
	glVertex3f(65.0, 60.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glutSwapBuffers();
}


void cloth3(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glVertex3f(25.0, 120.0, 0.0);
	glVertex3f(15.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(75.0, 120.0, 0.0);
	glVertex3f(85.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 10.0, 0.0);
	glVertex3f(65.0, 10.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glutSwapBuffers();
}


void cloth4(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glVertex3f(30.0, 120.0, 0.0);
	glVertex3f(20.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(70.0, 120.0, 0.0);
	glVertex3f(80.0, 130.0, 0.0);
	glEnd();


	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 60.0, 0.0);
	glVertex3f(65.0, 60.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glutSwapBuffers();
}


void cloth5(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 150.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glVertex3f(35.0, 75.0, 0.0);
	glVertex3f(30.0, 80.0, 0.0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(65.0, 150.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(65.0, 75.0, 0.0);
	glVertex3f(70.0, 80.0, 0.0);
	glEnd();


	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(35.0, 60.0, 0.0);
	glVertex3f(65.0, 60.0, 0.0);
	glVertex3f(60.0, 140.0, 0.0);
	glVertex3f(40.0, 140.0, 0.0);
	glEnd();


	glutSwapBuffers();
}


void display21(void)
{
	//int i;
	int R = 20;
	int PI = 3;
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	cloth5();
	//画点程序
	//glPointSize(10);   // 绘制前设置下点的大小和颜色
	//glColor3f(1, 0, 0);
	//glBegin(GL_POINTS);
	//glVertex3f(50, 50, 20);
	//glEnd();


	//glBegin(GL_POINTS);
	//glVertex2f(0.0f,0.0f);
	//glVertex2f(0.5f,0.5f);
	//glEnd();


	//glLineWidth(5);
	//glBegin(GL_LINE_LOOP);
	//for(i=0; i<100; i++)
	//{
	// glVertex3f(30+R*cos(2*PI/100*i),40+R*sin(2*PI/100*i),20);
	 //觉得数学有压力就多练练
	//}
	//glEnd();
	//glLineWidth(1);


	//画直线程序
	//glColor3f(0, 2, 0);
	//glBegin(GL_LINES);
	//glVertex3f(80.0, 0.0, 0.0);
	//glVertex3f(80.0, 87, 27);
	//glEnd();

	//glBegin(GL_LINES);
		//glVertex2f(0.0f,0.3f);
		//glVertex2f(0.3f,0.5f);
		//glEnd();


	//画三角形
	//glBegin(GL_TRIANGLES);                          // 绘制三角形
	//glColor3f(0, 0, 0);
	//glVertex3f(50.0, 30.0, 0.0);                    // 上顶点
	//glVertex3f(50.0, 20.0, 0.0);                    // 左下
	//glVertex3f(20.0, 10.0, 0.0);                    // 右下
	//glEnd();


	//画矩形
	// glBegin(GL_QUADS);                            //  绘制正方形
	// glVertex3f(30.0, 65.0, 30.0);                    // 左上
	// glVertex3f(65.0, 65.0, 20.0);                    // 右上
	// glVertex3f(30.0, 35.0, 30.0);                    // 左下
	// glVertex3f(65.0, 35.0, 50.0);                    // 右下
	// glEnd();


	//画的位置移动的函数
	//glTranslatef(3.0f, 0.0f, 0.0f);                        // 右移3单位


	// 只flush没用的，直接swap就好了
	//glutSwapBuffers();
}


void reshape21(int w, int h)
{
	// 这里要设置opengl在窗口的绘图区域，一般是整个窗口
	glViewport(0, 0, w, h);


	// 一般只在窗口reshape时才重新设定下投影矩阵和模型矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// 镜头默认在坐标原点看向Z轴负方向，而glOrtho最后两个参数表示
	// 距离镜头zNear到zFar距离间的图形才可以看到
	// 而且默认坐标系Z轴正方向是指向屏幕朝外侧的，
	// 你设置成0, 40就只能看到Z坐标( -40, 0 )间的物体，
	// 所以zNear给个负数，就能看到Z坐标为正的物体了
	glOrtho(0, 100, 0, 200, -40, 40);


	// 投影矩阵设好后再将当前矩阵设置为模型矩阵，
	// 方便后续的图形绘制和图形变换
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			// num ++;
			// num=num%3;
		}
		glutIdleFunc(cloth1);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			;
		}
		glutIdleFunc(cloth2);
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	default:
		break;
	}
	/*
	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(50);   // 绘制前设置下点的大小和颜色
	glBegin(GL_POINTS);
	switch(num){
	case 1:
	glColor3f(0, 0, 1);
	break;
	case 2:
	glColor3f(0, 1, 0);
	break;
	case 0:
	glColor3f(1, 0, 0);
	break;
	}
	glVertex3f(56, 87, 27);
	glEnd();
	glutSwapBuffers();
	}
	*/
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("图形学作业");
	glClearColor(0.9, 0.9, 0.9, 1);
	glutDisplayFunc(display21);
	glutReshapeFunc(reshape21);// 这个必须有
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}