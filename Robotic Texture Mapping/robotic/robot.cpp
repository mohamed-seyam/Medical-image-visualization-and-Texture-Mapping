#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"
#include "glm.h"

#define PI 3.14

double eye[] = { 0.0, 0.0, 5.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0.0, 1.0, 0.0 };


const char* groundname = "images/floor1.bmp";
char *modelname = (char*) "data/ballonFoot.obj";


GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 0.5, 5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 1.0 };
static int LeftShoulderX = 0, LeftShoulderY = 0, LeftShoulderZ = -80 , RightShoulderX = 0, RightShoulderY = 0, RightShoulderZ = 80, LeftElbow = 0, RightElbow = 0, LeftUpperLegX = 0, LeftUpperLegZ = 0, LeftLowerLeg = 0, RightUpperLegX = 0, RightUpperLegZ = 0, RightLowerLeg = 0, BallForward = 2.4, BallUp = -4;
int moving, startx, starty;
//int poses[11][14] = {{0,0,-80,0,0,80,0,0,0,0,0,25,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,20,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,15,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,10,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,5,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,0,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,-5,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,-10,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,-15,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,-20,0,0}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0}};

int poses[12][16] = { {0,0,-80,0,0,80,0,0,0,0,0,40,0,0, 2.4, -4} , {0,0,-80,0,0,80,0,0,0,0,0,25,0,0, 2.4, -4}, {0,0,-80,0,0,80,0,0,0,0,0,5,0,0, 2.4, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-5,0,0, 2.4, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-15,0,0, 2.4, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 2.4, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 3, -3.8}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 4, -3.7}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 5, -3.9}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 6, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 7, -4}, {0,0,-80,0,0,80,0,0,0,0,0,-25,0,0, 8, -4} };
GLfloat angle = 0.0;
GLfloat angle2 = 0.0;   

void drawmodel(char* filename)
{
    GLMmodel* model = glmReadOBJ(filename);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90.0);
    //glmScale(model, 0.51);
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}


GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);			
    glBindTexture(GL_TEXTURE_2D, textureId); 
    glTexImage2D(GL_TEXTURE_2D,				
        0,							
        GL_RGB,					 
        image->width, image->height, 
        0,						
        GL_RGB,					 
        GL_UNSIGNED_BYTE,			 
                                    
        image->pixels);			
    return textureId;						
}

GLuint _textureId;

void initRendering()
{
    Image* image = loadBMP(groundname);
    _textureId = loadTexture(image);
    delete image;
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}


void draw_ground() {
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int z = 0; z < 20; z++) {
        float zStart = 100.0f - z * 10.0f;
        for (int x = 0; x < 20; x++) {
            float xStart = x * 10.0f - 100.0f;

            glTexCoord2f(0.0f, 0.0f);  glVertex3f(xStart, -5.0f, zStart);
            glTexCoord2f(3.0f, 0.0f);  glVertex3f(xStart + 10.0f, -5.0f, zStart);
            glTexCoord2f(3.0f, 3.0f);  glVertex3f(xStart + 10.0f, -5.0f, zStart - 10.0f);
            glTexCoord2f(0.0f, 3.0f);  glVertex3f(xStart, -5.0f, zStart - 10.0f);
        }
    }

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}


void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}


void lookRight()
{
    rotatePoint(up, PI / 8, eye);
}


void lookLeft()
{
    rotatePoint(up, -PI / 8, eye);

}


void lookUp()
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, PI / 8, eye);
    rotatePoint(horizontal, PI / 8, up);
}


void lookDown()
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -PI / 8, eye);
    rotatePoint(horizontal, -PI / 8, up);

}


void moveForward()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    float speed = 0.1;
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}


void moveBackward()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    float speed = 0.1;
    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;

    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;
}


void init(void)
{
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0,
        1.0,
        1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


void reset()
{
    double e[] = { 0.0, 0.0, 5.0 };
    double c[] = { 0.0, 0.0, 0.0 };
    double u[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = e[i];
        center[i] = c[i];
        up[i] = u[i];
    }
}


void screen_menu(int value)
{

    switch (value)
    {
    case '0':
        groundname = "images/floor1.bmp";
        std::cout << groundname << std::endl;
        initRendering();

        break;
    case '1':
        groundname = "images/floor2.bmp";
        std::cout << groundname << std::endl;
        initRendering();
        break;
    case '2':
        groundname = "images/floor3.bmp";
        std::cout << groundname << std::endl;
        initRendering();
        break;
    }

    reset();
    glutPostRedisplay();
}


void setposes(int frameNum) {
    LeftShoulderX = poses[frameNum][0];
    LeftShoulderY = poses[frameNum][1];
    LeftShoulderZ = poses[frameNum][2];
    RightShoulderX = poses[frameNum][3];
    RightShoulderY = poses[frameNum][4];
    RightShoulderZ = poses[frameNum][5];
    LeftElbow = poses[frameNum][6];
    RightElbow = poses[frameNum][7];
    LeftUpperLegX = poses[frameNum][8];
    LeftUpperLegZ = poses[frameNum][9];
    LeftLowerLeg = poses[frameNum][10];
    RightUpperLegX = poses[frameNum][11];
    RightUpperLegZ = poses[frameNum][12];
    RightLowerLeg = poses[frameNum][13];
    BallForward = poses[frameNum][14];
    BallUp = poses[frameNum][15];
}

static int f = 0;


void timer(int value) {
    f = f % 12;
    setposes(f);
    f++;
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}


void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        lookLeft();
        break;
    case GLUT_KEY_RIGHT:
        lookRight();
        break;
    case GLUT_KEY_UP:
        lookUp();
        break;
    case GLUT_KEY_DOWN:
        lookDown();
        break;
    }
    glutPostRedisplay();
}


void create_object(void) {

    glPushMatrix();                 //Head
    glTranslatef(0.0, 1.85, 0.0);
    glutWireSphere(0.35, 50.0, 50.0);

    glPopMatrix();

    glPushMatrix();                 //Trunk
    glScalef(1.5, 3, 0.5);
    glColor3f(1, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                    //Left upper arm
    glTranslatef(0.675, 1.25, 0.0);     //Here we split the translation before and after the rotation in order to perform fixed point rotation
    glRotatef((GLfloat)LeftShoulderX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)LeftShoulderY, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)LeftShoulderZ, 0.0, 0.0, 1.0);
    glTranslatef(0.675, 0.0, 0.0);
    glScalef(1.2, 0.1, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Left lower arm
    glTranslatef(0.675, 1.25, 0.0);
    glRotatef((GLfloat)LeftShoulderX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)LeftShoulderY, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)LeftShoulderZ, 0.0, 0.0, 1.0);
    glTranslatef(0.675, 0.0, 0.0);
    glTranslatef(0.6, 0.0, 0.0);
    glRotatef((GLfloat)LeftElbow, 0.0, 1.0, 0.0);
    glTranslatef(0.6, 0.0, 0.0);
    glScalef(1.2, 0.1, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Right upper arm
    glTranslatef(-0.675, 1.25, 0.0);
    glRotatef((GLfloat)RightShoulderX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)RightShoulderY, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)RightShoulderZ, 0.0, 0.0, 1.0);
    glTranslatef(-0.675, 0.0, 0.0);
    glScalef(1.2, 0.1, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Right lower arm
    glTranslatef(-0.675, 1.25, 0.0);
    glRotatef((GLfloat)RightShoulderX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)RightShoulderY, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)RightShoulderZ, 0.0, 0.0, 1.0);
    glTranslatef(-0.675, 0.0, 0.0);
    glTranslatef(-0.6, 0.0, 0.0);
    glRotatef((GLfloat)RightElbow, 0.0, 1.0, 0.0);
    glTranslatef(-0.6, 0.0, 0.0);
    glScalef(1.2, 0.1, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Left upper Leg
    glTranslatef(0.5, -1.625, 0.0);
    glRotatef((GLfloat)LeftUpperLegX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)LeftUpperLegZ, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.625, 0.0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Left lower Leg
    glTranslatef(0.5, -1.625, 0.0);
    glRotatef((GLfloat)LeftUpperLegX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)LeftUpperLegZ, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.625, 0.0);
    glTranslatef(0.0, -0.75, 0.0);
    glRotatef((GLfloat)LeftLowerLeg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.75, 0.0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glTranslatef(0.0, -0.55, 0.0);
    glScalef(1.0, 0.15, 2.25);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Right upper Leg
    glTranslatef(-0.5, -1.625, 0.0);
    glRotatef((GLfloat)RightUpperLegX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)RightUpperLegZ, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.625, 0.0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Right lower Leg
    glTranslatef(-0.5, -1.625, 0.0);
    glRotatef((GLfloat)RightUpperLegX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)RightUpperLegZ, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.625, 0.0);
    glTranslatef(0.0, -0.75, 0.0);
    glRotatef((GLfloat)RightLowerLeg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.75, 0.0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glTranslatef(0.0, -0.55, 0.0);
    glScalef(1.0, 0.15, 2.25);
    glutSolidCube(1.0);
    glPopMatrix();



    glPopMatrix();

}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]);
    glClear(GL_COLOR_BUFFER_BIT);
    //texture_object();
    glPushMatrix();
    //glutWireSphere(1.0, 1.0, 1.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.25, 0.0);
    //glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
    //glTranslatef(1.0, 0.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    draw_ground();
    glPushMatrix();
    glTranslatef(0, BallUp, BallForward);
    //glScalef(15, 15, 15);
    drawmodel(modelname);
    glPopMatrix();
    create_object();
    glutSwapBuffers();

}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 3.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        if (LeftShoulderX == -30) { break; }
        LeftShoulderX = (LeftShoulderX - 5) % 61;
        std::cout << "leftshoulderx" << LeftShoulderX << std::endl;
        glutPostRedisplay();
        break;
    case '!':
        if (LeftShoulderX == 60) { break; }
        LeftShoulderX = (LeftShoulderX + 5) % 61;
        glutPostRedisplay();
        break;
    case '2':
        if (LeftShoulderY == -150) { break; }
        LeftShoulderY = (LeftShoulderY - 5) % 151;
        std::cout << "leftshouldery" << LeftShoulderY << std::endl;
        glutPostRedisplay();
        break;
    case '@':
        if (LeftShoulderY == 30) { break; }
        LeftShoulderY = (LeftShoulderY + 5) % 151;
        glutPostRedisplay();
        break;
    case '3':
        if (LeftShoulderZ == 90) { break; }
        LeftShoulderZ = (LeftShoulderZ + 5) % 151;
        std::cout << "leftshoulderz" << LeftShoulderZ << std::endl;
        glutPostRedisplay();
        break;
    case '#':
        if (LeftShoulderZ == -85) { break; }
        LeftShoulderZ = (LeftShoulderZ - 5) % 151;
        std::cout << "leftshoulderz" << LeftShoulderZ << std::endl;
        glutPostRedisplay();
        break;

    case 'b':
        if (LeftElbow == -120) { break; }
        LeftElbow = (LeftElbow - 5) % 121;
        std::cout << "leftelbow" << LeftElbow << std::endl;
        glutPostRedisplay();
        break;
    case 'B':
        if (LeftElbow == 0) { break; }
        LeftElbow = (LeftElbow + 5) % 121;
        std::cout << "leftelbow" << LeftElbow << std::endl;
        glutPostRedisplay();
        break;
    case '4':
        if (RightShoulderX == -30) { break; }
        RightShoulderX = (RightShoulderX - 5) % 61;
        std::cout << "Rightsholderx" << RightShoulderX << std::endl;
        glutPostRedisplay();
        break;
    case '$':
        if (RightShoulderX == 60) { break; }
        RightShoulderX = (RightShoulderX + 5) % 61;
        std::cout << "Rightsholderx" << RightShoulderX << std::endl;
        glutPostRedisplay();
        break;
    case '5':
        if (RightShoulderY == -30) { break; }
        RightShoulderY = (RightShoulderY - 5) % 151;
        std::cout << "RightsholderY" << RightShoulderY << std::endl;
        glutPostRedisplay();
        break;
    case '%':
        if (RightShoulderY == 150) { break; }
        RightShoulderY = (RightShoulderY + 5) % 151;
        std::cout << "RightsholderY" << RightShoulderY << std::endl;
        glutPostRedisplay();
        break;
    case '6':
        if (RightShoulderZ == 90) { break; }
        RightShoulderZ = (RightShoulderZ + 5) % 151;
        std::cout << "Rightsholderz" << RightShoulderZ << std::endl;
        glutPostRedisplay();
        break;
    case '^':
        if (RightShoulderZ == -85) { break; }
        RightShoulderZ = (RightShoulderZ - 5) % 151;
        std::cout << "Rightsholderz" << RightShoulderZ << std::endl;
        glutPostRedisplay();
        break;
    case '7':
        if (RightElbow == 120) { break; }
        RightElbow = (RightElbow + 5) % 121;
        std::cout << "RightElbow" << RightElbow << std::endl;
        glutPostRedisplay();
        break;
    case '&':
        if (RightElbow == 0) { break; }
        RightElbow = (RightElbow - 5) % 121;
        std::cout << "RightElbow" << RightElbow << std::endl;
        glutPostRedisplay();
        break;
    case '8':
        if (LeftUpperLegX == -80) { break; }
        LeftUpperLegX = (LeftUpperLegX - 5) % 81;
        std::cout << "LeftUpperLegx" << LeftUpperLegX << std::endl;
        glutPostRedisplay();
        break;
    case '*':
        if (LeftUpperLegX == 80) { break; }
        LeftUpperLegX = (LeftUpperLegX + 5) % 81;
        std::cout << "LeftUpperLegx" << LeftUpperLegX << std::endl;
        glutPostRedisplay();
        break;
    case '9':
        if (LeftUpperLegZ == 0) { break; }
        LeftUpperLegZ = (LeftUpperLegZ - 5) % 81;
        std::cout << "LeftUpperLegZ" << LeftUpperLegZ << std::endl;
        glutPostRedisplay();
        break;
    case '(':
        if (LeftUpperLegZ == 80) { break; }
        LeftUpperLegZ = (LeftUpperLegZ + 5) % 81;
        std::cout << "LeftUpperLegZ" << LeftUpperLegZ << std::endl;
        glutPostRedisplay();
        break;
    case 'q':
        if (LeftLowerLeg == 0) { break; }
        LeftLowerLeg = (LeftLowerLeg - 5) % 81;
        std::cout << "LeftLowerLeg" << LeftLowerLeg << std::endl;
        glutPostRedisplay();
        break;
    case 'Q':
        if (LeftLowerLeg == 80) { break; }
        LeftLowerLeg = (LeftLowerLeg + 5) % 81;
        std::cout << "LeftLowerLeg" << LeftLowerLeg << std::endl;
        glutPostRedisplay();
        break;
    case 'w':
        if (RightUpperLegX == -80) { break; }
        RightUpperLegX = (RightUpperLegX - 5) % 81;
        std::cout << "RightUpperLegX" << RightUpperLegX << std::endl;
        glutPostRedisplay();
        break;
    case 'W':
        if (RightUpperLegX == 80) { break; }
        RightUpperLegX = (RightUpperLegX + 5) % 81;
        std::cout << "RightUpperLegX" << RightUpperLegX << std::endl;
        glutPostRedisplay();
        break;
    case 'e':
        if (RightUpperLegZ == 0) { break; }
        RightUpperLegZ = (RightUpperLegZ + 5) % 81;
        std::cout << "RightUpperLegZ" << RightUpperLegZ << std::endl;
        glutPostRedisplay();
        break;
    case 'E':
        if (RightUpperLegZ == -80) { break; }
        RightUpperLegZ = (RightUpperLegZ - 5) % 81;
        std::cout << "RightUpperLegZ" << RightUpperLegZ << std::endl;
        glutPostRedisplay();
        break;
    case 'a':
        if (RightLowerLeg == 0) { break; }
        RightLowerLeg = (RightLowerLeg - 5) % 81;
        std::cout << "RightLowerLeg" << RightLowerLeg << std::endl;
        glutPostRedisplay();
        break;
    case 'A':
        if (RightLowerLeg == 80) { break; }
        RightLowerLeg = (RightLowerLeg + 5) % 81;
        std::cout << "RightLowerLeg" << RightLowerLeg << std::endl;
        glutPostRedisplay();
        break;
    case '+':
        moveForward();
        break;
    case '-':
        moveBackward();
        break;
    case 'R':
        reset();
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}


static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}


static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}


void attachMenu()
{

    /*std::cout << "iam here" << std::endl;*/
    glutCreateMenu(screen_menu);
    glutAddMenuEntry("floor1", '0');
    glutAddMenuEntry("floor2", '1');
    glutAddMenuEntry("floor3", '2');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 1100);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Task 3");
    init();
    initRendering();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    attachMenu();
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}