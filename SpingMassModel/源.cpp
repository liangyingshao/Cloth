#ifndef GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif
#define GLEW_STATIC
#include <GL/freeglut.h>
#include <vector>
#include<cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for matrices
#include <glm/gtc/type_ptr.hpp>
//undefine if u want to use the default bending constraint of pbd
#include<iostream>
using namespace std;
//using namespace glm;

GLfloat rtx = 0.0f, rty = 0.0f, rtz = 0.0;
GLfloat sim_step = 0.1;

glm::vec3 Up = glm::vec3(0, 1, 0), Right, viewDir;
const int GRID_SIZE = 10;  //地板砖边长
GLdouble MV[16];
GLint viewport[4];
GLdouble PP[16];
bool isfix[1000];
int dist = -23;

//椭球参数
int iStacks = 30;
int iSlices = 30;
float fRadius = 1;
//float exp1 = 1e-3;

const int numX = 20, numY = 20; //一行有numx+1个点
const int total_points = (numX + 1) * (numY + 1); //总点数
//布料顶点位置 速度
const int fck = 10000;
glm::vec3 Pos[total_points];
glm::vec3 Veloc[total_points];
glm::vec3 force[total_points];
int mySize = 4;
float hsize = mySize / 2.0f;
float currentTime, newTime, startTime;

const float mass = 1.0 / total_points;
const glm::vec3 gvat = glm::vec3(0, -9.8, 0);  //重力加速度
const float Spring_K = 2.5;  //弹性系数
const float len0 = 4.0 / numX; //单边长度
const float tolera = 1.08;   //弹性限度

void initGL()
{
    //初始化顶点位置
    memset(Pos, 0, sizeof(Pos));
    memset(Veloc, 0, sizeof(Veloc));
    memset(force, 0, sizeof(force));
    //fill in positions
    int count1 = 0;
    int u = numX + 1;
    int v = numY + 1;
    for (int j = 0; j <= numY; j++) {
        for (int i = 0; i <= numX; i++) {
            Pos[count1++] = glm::vec3(((float(i) / (u - 1)) * 2 - 1) * hsize, mySize + 1, ((float(j) / (v - 1)) * mySize));
            //printf("(%.1lf ,%.1lf)", ((float(i) / (u - 1)) * 2 - 1) * hsize, ((float(j) / (v - 1)) * mySize));
        }//printf("\n");
        //悬挂点为X[0] 和 X[numX]
    }
    memset(isfix, 0, sizeof(isfix));
    isfix[0] = isfix[numX] = 1;

    //处理时间
    startTime = (float)glutGet(GLUT_ELAPSED_TIME);
    cout << startTime << "ddddddddddd\n";
    currentTime = startTime;
}

void DrawGrid()
{
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
    {
        glVertex3f((float)i, 0, (float)-GRID_SIZE);
        glVertex3f((float)i, 0, (float)GRID_SIZE);

        glVertex3f((float)-GRID_SIZE, 0, (float)i);
        glVertex3f((float)GRID_SIZE, 0, (float)i);
    }
    glEnd();
}

void drawTextile() {
    //draw polygons
    int k = 0;
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    for (int i = 0; i <= numX; i++) {
        for (int j = 0; j <= numY; j++) {
            //                cout<<i<<" "<<j<<";";
            if (j != numX) {
                glm::vec3 p1 = Pos[k];
                glm::vec3 p2 = Pos[k + 1];
                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
                //                printf("add %d %d\n",k,k+1);
            }
            if (i != numY) {
                glm::vec3 p1 = Pos[k];
                glm::vec3 p2 = Pos[k + numX + 1];
                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
            }
            k++;
        }
        //        cout<<endl;
    }
    glEnd();

    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    for (size_t i = 0; i < total_points; i++) {
        glm::vec3 p = Pos[i];
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

void OnRender(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //设置视角
    glTranslatef(0, 0, dist);
    glRotatef(15, 1, 0, 0);
    //做出地面
    //DrawGrid();
    drawTextile();//画布料
    //    drawblock();
    glutSwapBuffers();
}
//求出b点对a点的作用力,用了三种弹簧
glm::vec3 SpringForce(int a, int b, int is) {
    float inilen;
    if (is == 1)inilen = len0;
    if (is == 2) inilen = len0 * 2;
    if (is == 3) inilen = len0 * 1.414213;
    glm::vec3 res = glm::vec3(0);
    glm::vec3 tmp = Pos[b] - Pos[a];
    float dis = glm::length(tmp);
    res = tmp / dis;
    res *= (dis - inilen);
    res *= Spring_K;
    return res;
}

void ComputeForces() {
    //重力
    for (size_t i = 0; i < total_points; i++) {
        force[i] = glm::vec3(0);
        if (!isfix[i])
            force[i] += mass * gvat;
    }
    int i, j, k = 0;
    //结构弹簧 上下左右
    for (i = 0; i <= numY; i++) {
        for (j = 0; j <= numX; j++) {
            if (i != 0) {//上
                force[k] += SpringForce(k, k - numX - 1, 1);
            }
            if (j != numX) {//右
                force[k] += SpringForce(k, k + 1, 1);
            }
            if (i != numY) {//下
                force[k] += SpringForce(k, k + numX + 1, 1);
            }
            if (j != 0) {//左
                force[k] += SpringForce(k, k - 1, 1);
            }
            k++;
        }
    }
    //柔性弹簧
    k = 0;
    for (i = 0; i <= numY; i++) {
        for (j = 0; j <= numX; j++) {
            if (i > 1) {//上
                force[k] += SpringForce(k, k - 2 * numX - 2, 2);
            }
            if (j < numX - 1) {//右
                force[k] += SpringForce(k, k + 2, 2);
            }
            if (i < numY - 1) {//下
                force[k] += SpringForce(k, k + 2 * numX + 2, 2);
            }
            if (j > 1) {//左
                force[k] += SpringForce(k, k - 2, 2);
            }
            k++;
        }
    }
    //剪切弹簧
    k = 0;
    for (i = 0; i <= numY; i++) {
        for (j = 0; j <= numX; j++) {
            if (i > 0 && j > 0) {  //左上
                force[k] += SpringForce(k, k - numX - 2, 3);
            }
            if (i > 0 && j < numX) {  //右上
                force[k] += SpringForce(k, k - numX, 3);
            }
            if (i < numY && j < numX) {  //右下
                force[k] += SpringForce(k, k + numX + 2, 3);
            }
            if (i < numY && j>0) {   //坐下
                force[k] += SpringForce(k, k + numX, 3);
            }
            k++;
        }
    }
}

void CalcPos() {  //计算新的位置
    newTime = (float)glutGet(GLUT_ELAPSED_TIME);
    float frameTime = newTime - currentTime;
    frameTime /= 1000;
    currentTime = newTime;
    glm::vec3 acc = glm::vec3(0);
    //得到的frame就是间隔时间 毫秒
    for (size_t i = 0; i <= total_points; i++) {
        if (isfix[i])continue;
        acc = force[i] / mass;  //得到加速度向量
        Veloc[i] *= 0.98;
        Veloc[i] = Veloc[i] + acc * frameTime;  //得到新的速度值
        Pos[i] = Pos[i] + Veloc[i] * frameTime;
    }
}

void gao(int a, int b, int is) {  //直线弹簧 要考虑是否是isfix！
    glm::vec3 tmp = Pos[b] - Pos[a];
    float dis = glm::length(tmp);
    float inilen;
    if (is == 1)inilen = len0;
    if (is == 2) inilen = len0 * 2;
    if (is == 3) inilen = len0 * 1.414213;
    inilen *= tolera;
    if (dis <= inilen)return;
    glm::vec3 tmp1 = tmp * (inilen / dis);
    if (isfix[a] && isfix[b]) {
        return;
    }
    else if (isfix[a]) {
        Pos[b] = Pos[a] + tmp1;
    }
    else if (isfix[b]) {
        Pos[a] = Pos[b] - tmp1;
    }
    else {
        glm::vec3 mid = (Pos[a] + Pos[b]) * 0.5f;
        Pos[a] = mid - tmp1 * 0.5f;
        Pos[b] = mid + tmp1 * 0.5f;
    }
}

//限制每一根弹簧长度
void DynamicConstrain() {
    int i, j, k = 0;
    for (i = 0; i <= numY; i++) {
        for (j = 0; j <= numX; j++) {
            if (j != numX) {//右
                gao(k, k + 1, 1);
            }
            if (i != numY) {//下
                gao(k, k + numX + 1, 1);
            }


            if (j < numX - 1) {//右二
                gao(k, k + 2, 2);
            }
            if (i < numY - 1) {//下二
                gao(k, k + 2 * numX + 2, 2);
            }


            if (i < numY && j < numX) {  //右下
                gao(k, k + numX + 2, 3);
            }
            if (i < numY && j>0) {   //左下
                gao(k, k + numX, 3);
            }
            k++;
        }
    }
}

int temp = 0;
void StepPhysics() {
    ComputeForces();
    CalcPos();
    glutPostRedisplay();
    //DynamicConstrain();
    Sleep(5);
}


void OnReshape(int nw, int nh) {
    glViewport(0, 0, nw, nh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)nw / (GLfloat)nh, 1.f, 100.0f);

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, PP);

    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("Mass-Spring Model");

    initGL();
    glutDisplayFunc(OnRender);
    //指定窗口形状变化时的回调函数
    glutReshapeFunc(OnReshape);
    //指定程序空闲时调用函数
    glutIdleFunc(StepPhysics);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
