/*#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GL/glu.h"
//glm not required
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
#include <string>*/

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdio.h>
#include <string>
#include <cstring>
#include <list>
#include <time.h>
#include "tga.h"
#include "shaderprogram.h"

#include "Model.h"

Model lego;
int win_width = 500, win_height = 500;


std::vector< glm::vec3 > vertices1;
std::vector< glm::vec2 > uvs1;
std::vector< glm::vec3 > normals1;

std::vector< float > verticesf;
std::vector< float > uvsf;
std::vector< float > normalsf;

float tabv[1000000];
float tabn[1000000];
float tabt[1000000];


float rotY = 0.0;
float rotX = 0.0;
float odleglosc=0.0;
int rpmdown,lpmdown;
int g_yClick,g_xClick,g_yClick2;

void processPick (int x, int y)
{
	GLint viewport[4];
	GLubyte pixel[3];

	glGetIntegerv(GL_VIEWPORT,viewport);

	glReadPixels(x,viewport[3]-y,1,1,
		GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

	printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	
  printf ("\n");
   
}

void vectofl()
{
	for(int i=0;i<vertices1.size();i++)
	{
		verticesf.push_back(vertices1[i].x);
		verticesf.push_back(vertices1[i].y);
		verticesf.push_back(vertices1[i].z);
		verticesf.push_back(1.0);
	}
	for(int i=0;i<normals1.size();i++)
	{
		normalsf.push_back(normals1[i].x);
		normalsf.push_back(normals1[i].y);
		normalsf.push_back(normals1[i].z);
		normalsf.push_back(0.0);
	}
	for(int i=0;i<uvs1.size();i++)
	{
		uvsf.push_back(uvs1[i].x);
		uvsf.push_back(uvs1[i].y);
	}
}
void vectotabo(float tab1[],float tab2[],float tab3[])
{
	vectofl();
	for (int i=0;i<verticesf.size();i++){
		tab1[i]=verticesf[i];}
	for( int i=0;i<normalsf.size();i++){
		tab2[i]=normalsf[i];}
	for( int i=0;i<uvsf.size();i++){
		tab3[i]=uvsf[i];}
}


void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		rpmdown = (state == GLUT_DOWN) ? 1 : 0;
		g_yClick = y - 3 * rotY;
		g_xClick = x - 3 * rotX;
		//printf("odleglosc = %f\n", odleglosc);
	}
	else if (button == 3)
	{
		odleglosc+=0.1;
	}
	else if (button == 4)
	{
		odleglosc-=0.1;
	}
	if (button == GLUT_LEFT_BUTTON)
	{
		printf("CLICKED LEFT BUTTON \n");
		processPick(x,y);
	}
}

void MouseMotion(int x, int y){
	if (rpmdown)
	{
		rotY = (y - g_yClick) / 3.0;
		rotX = (x - g_xClick) / 3.0;
		glutPostRedisplay();
	}
	else if(lpmdown){
		odleglosc = (y - g_yClick2) / 3.0;
	}
}


void renderScene(void) {
    static float dis=0, ddis=0, elev=0, delev=0, azim=0, dazim=0;

    azim += 0.5f;
    if (azim >= 360.0f){
        azim -= 360.0f;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, odleglosc);
    glRotated(rotY, 1.0, 0.0, 0.0);
    glRotated(rotX, 0.0, 1.0, 0.0);
    glScalef(0.2f,0.2f,0.2f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
	for(int i=0; i<vertices1.size();i++){
        	glVertex3f(vertices1[i].x, vertices1[i].y, vertices1[i].z);
	}
/*        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);*/
    glEnd();


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();



    //I like to use glm because glu is deprecated
    //glm::mat4 orth= glm::ortho(0.0f, (float)win_width, 0.0f, (float)win_height);
    //glMultMatrixf(&(orth[0][0]));
    gluOrtho2D(0.0, win_width, 0.0, win_height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 0.0f);//needs to be called before RasterPos
    glRasterPos2i(10, 10);
    std::string s = "Some text";
    void * font = GLUT_BITMAP_9_BY_15;

    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0); 
        glutBitmapCharacter(font, c);
    }

glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0);
    glVertex2f(0.0, 0.0);
    glVertex2f(100.0, 0.0);
    glVertex2f(100.0, 100.0);
    glVertex2f(0.0, 100.0);
glEnd();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

    glutSwapBuffers();
    glutPostRedisplay();
}

void gloadobj(const char * path, Model * mod, float tab1[], float tab2[], float tab3[]){

	vertices1.clear();uvs1.clear();normals1.clear();
	verticesf.clear();uvsf.clear();normalsf.clear();

	loadOBJ(path,vertices1, uvs1, normals1);
	vectotabo(tab1,tab2,tab3);
	mod->vertexCount=verticesf.size()/4;
	//printf("%d\n",kostka.vertexCount);
	mod->vertices = tab1;
	mod->normals = tab2;
	mod->texCoords = tab3;
}



int main(int argc, char **argv) {
	gloadobj("objs/skeleton.obj",&lego,tabv,tabn,tabt);
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("GLUT Test");

    // register callbacks
    glutDisplayFunc(renderScene);
	glutMouseFunc (MouseButton);
  	glutMotionFunc (MouseMotion);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
