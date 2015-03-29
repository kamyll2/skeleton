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

#define VIEWING_DISTANCE_MIN  3.0

int counter=0,counter1=1;
int ROUND=0;

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

void MouseButton(int button, int state, int x, int y)
{
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.

  if (button == GLUT_RIGHT_BUTTON)
    {
      rpmdown = (state == GLUT_DOWN) ? 1 : 0;
	g_yClick = y - 3 * rotY;
	g_xClick = x - 3 * rotX;
      //printf("odleglosc = %f\n", odleglosc);
    }
  else if (button == 3)
    {
	odleglosc++;
    }
  else if (button == 4)
    {
	odleglosc--;
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

//Macierze
glm::mat4  matP;//rzutowania
glm::mat4  matV;//widoku
glm::mat4  matM;//modelu


//Ustawienia okna i rzutowania
int windowPositionX=100;
int windowPositionY=100;
int windowWidth=800;
int windowHeight=800;
float cameraAngle=50.0f;

//Zmienne do animacji
float speed=120; //120 stopni/s
int lastTime=0;
float angle=0;
Model lego;

//Procedura rysuj¹ca jakiœ obiekt. Ustawia odpowiednie parametry dla vertex shadera i rysuje.
void drawObject(Model mod) {

	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(matV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(matM));
	
	mod.draw();
}
void drawTex(Model mod,GLuint tex) {

	shaderProgram->use();

	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(matV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(matM));
	
	mod.drawtex(tex);
}
//Procedura rysuj¹ca
void displayFrame() {

	matV=glm::lookAt(glm::vec3(0.0f,0.0f,9.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

	glClearColor(0.25,1.0,0.0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

 
	matM=glm::mat4(1.0f) ;

	matM=glm::scale(matM, glm::vec3(0.2,0.2,0.2));
	matM=glm::translate(matM,glm::vec3(0.0,0.0,odleglosc));
	matM=glm::rotate(matM,rotX,glm::vec3(0.0,1.0,0.0));
	matM=glm::rotate(matM,rotY,glm::vec3(1.0,0.0,0.0));


	//Narysuj obiekt
	drawObject(lego);

//	matM=matS;

	glutSwapBuffers();
}


void setupVBO() {
lego.setupVBO();
}


void setupVAO() {
lego.setupVAO();
}


void nextFrame(void) {	
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	//cout<<interval<<endl;
	lastTime=actTime;

	glutPostRedisplay();
}


void changeSize(int w, int h) {
	
	glViewport(0,0,w,h);
	
	windowWidth=w;
	windowHeight=h;
}

void keyDown(int c, int x, int y)
{
	if(c==GLUT_KEY_RIGHT){}
	if(c==GLUT_KEY_LEFT){}
	if(c==GLUT_KEY_UP){}
	if(c==GLUT_KEY_DOWN){}
	glutPostRedisplay();

}

void keyup(int c, int x, int y)
{
	if(c==GLUT_KEY_RIGHT){}
	if(c==GLUT_KEY_LEFT){}

	glutPostRedisplay();

}

void initGLUT(int *argc, char** argv) {
	glutInit(argc,argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowPosition(windowPositionX,windowPositionY);
	glutInitWindowSize(windowWidth,windowHeight); 
	glutCreateWindow("OpenGL 3.3"); 
	glutReshapeFunc(changeSize); 
	glutDisplayFunc(displayFrame); 
	glutIdleFunc(nextFrame); 
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyup);
	glutMouseFunc (MouseButton);
  	glutMotionFunc (MouseMotion);

}

void initGLEW() {
	GLenum err=glewInit();
	if (GLEW_OK!=err) {
		fprintf(stderr,"%s\n",glewGetErrorString(err));
		exit(1);
	}
	
}



void setupShaders() {
	shaderProgram=new ShaderProgram("vshader.txt",NULL,"fshader.txt");
	shaderProgram->use();
}
GLuint readTexture(char* filename) {
	GLuint tex;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);
	if (img.Load(filename)==IMG_OK) {
		glGenTextures(1,&tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex
		if (img.GetBPP()==24) //Obrazek 24bit
			glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0,
			GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
		else if (img.GetBPP()==32) //Obrazek 32bit
			glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0,
			GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());      
		else {
			printf("Nieobs³ugiwany format obrazka w pliku: %s \n",filename);
		}
	} else {
		printf("B³¹d przy wczytywaniu pliku: %s\n",filename);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	return tex;
} 
void initOpenGL() {
	setupShaders();
	setupVBO();
	setupVAO();
		glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	lego.tex0=readTexture("tex/red.tga");
	lego.tex1=readTexture("tex/metal.tga");
}

//Zwolnij pamiêæ karty graficznej z shaderów i programu cieniuj¹cego
void cleanShaders() {
	delete shaderProgram;
}

void freeVBO() {
lego.freeVBO();
}

void freeVAO() {
lego.freeVAO();
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

int main(int argc, char** argv) {

	/*loadOBJ("objs/ludek.obj", vertices1, uvs1, normals1);
	//vectotab();	
	vectotabo(tabv,tabn,tabt);
	lego.vertexCount=verticesf.size()/4;
	lego.vertices=tabv;
	lego.normals=tabn;
	lego.texCoords=tabt;
	std::cout<<lego.vertexCount<<"\n";*/
	gloadobj("objs/skeleton.obj",&lego,tabv,tabn,tabt);

matP=glm::perspective(cameraAngle, (float)windowWidth/(float)windowHeight, 1.0f, 50.0f); 


	

	initGLUT(&argc,argv);
	initGLEW();
	initOpenGL();
	
	glutMainLoop();
	
	freeVAO();
	freeVBO();
	cleanShaders();
	return 0;
}
