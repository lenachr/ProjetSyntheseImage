#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "3D_tools.h"
#include "draw_scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "ball_racket.h"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "Main Menu";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
// static int flag_animate_rot_scale = 0;
// static int flag_animate_rot_arm = 0;

/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,aspectRatio,Z_NEAR,Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

// Pour permettre d'avancer en appuyant sur la touche flèche du haut
// int move = 1;
// double PausedTimeWalls = 0.0;
// double currentTimeWalls = 0.001;
// double lastPauseTime = 0.0;
int move = 0;
int firstStart = 0;
int firstStop = 0;
double currentTimeWalls = 1;
double pausedBegin = 1;
double pausedEnd = 0;
double pausedTime = 0;

//Gestions des différentes étapes : menu = 0, jeu = 1, fenêtre de fin echec = 2, fenêtre de fin reussite = 3
// On pourra juste avoir une image différente pour les deux fins avec les boutons et les fonctionnalités identiques
int mode = 0;

// Position des boutons du menu : 
int btn01Xmin = 340;
int btn01Xmax = 660;
int btn01Ymin = 298;
int btn01Ymax = 369;

int btn02Xmin = 340;
int btn02Xmax = 660;
int btn02Ymin = 398;
int btn02Ymax = 475;

int btn03Xmin = 340;
int btn03Xmax = 660;
int btn03Ymin = 500;
int btn03Ymax = 576;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	// On récupère la position de la souris
	double Xpos, Ypos;
	glfwGetCursorPos(window, &Xpos, &Ypos);
	
	// Si le mode de jeu est à 0 : on est donc dans le menu
	if(mode == 0){
		// Clic sur jouer
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn01Xmin && Xpos < btn01Xmax && Ypos > btn01Ymin && Ypos < btn01Ymax){
			mode = 1;
			printf("Jouer ! \n");
		}
		// Clic sur options
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn02Xmin && Xpos < btn02Xmax && Ypos > btn02Ymin && Ypos < btn02Ymax){
			mode = 2;
			printf("Options \n");
			
		}
		// Clic sur quitter
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn03Xmin && Xpos < btn03Xmax && Ypos > btn03Ymin && Ypos < btn03Ymax){
			// On ferme la fenêtre
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			printf("Quitter \n");
		}
	}
	// Si le mode de jeu est à 1 : on est donc dans le jeu
	if(mode == 1){
		
	}
	// Si le mode de jeu est à 2 ou 3 : on a fini le jeu
	if(mode == 2 || mode == 3){
		// Clic sur rejouer
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn02Xmin && Xpos < btn02Xmax && Ypos > btn02Ymin && Ypos < btn02Ymax){
			// mode = 0;
			printf("Options \n");
		}
		// Clic sur quitter
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn03Xmin && Xpos < btn03Xmax && Ypos > btn03Ymin && Ypos < btn03Ymax){
			// On ferme la fenêtre
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}	
	}
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_KP_9 :
				if(dist_zoom<100.0f) dist_zoom*=1.1;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_KP_3 :
				if(dist_zoom>1.0f) dist_zoom*=0.9;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_UP :
				// move = 1-move;
				// move = !move;
				// if (phy>2) phy -= 2;
				// printf("Phy %f\n",phy);
				move = 1;
            	// double currentPauseTime = glfwGetTime();
            	// currentTimeWalls += currentPauseTime - lastPauseTime;
            	// lastPauseTime = 0.0;
				break;
			case GLFW_KEY_DOWN :
				if (phy<=88.) phy += 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_LEFT :
				theta -= 5;
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
	if (action == GLFW_RELEASE){
		switch(key) {
			case GLFW_KEY_UP :
				move = 0;
            	// lastPauseTime = glfwGetTime();
				// move = 0;
				break;	
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

void erreur(unsigned char *image){
	if(image != NULL){
		printf("reussi \n");
	}else{
		printf("rate \n");

	}
}

// générer texture
GLuint loadTexture(const char* imageName){
	int x = 400;
	int y = 400;
	int n = 200;
	int c = 0;
	GLuint texture;
	glGenTextures(1, &texture);
	unsigned char* image = stbi_load(imageName, &x, &y, &n, c);
	glBindTexture(GL_TEXTURE_2D, texture);
	erreur(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	return texture;
}

//gére l'apparition du mur
// bool wall = true;

void speed_wall(float speed, int i){
		glPushMatrix();
			float color = 1-(speed/15.);
			// printf("couleur : %d \n", color);
			glColor3f(color,color,color); 
			glScalef(10,7,25);
				glTranslatef(-0.5,speed,0); 
			drawSquare();
		glPopMatrix();
}

void displayWalls(float* walls, float speed, double wallTime){
	float speeds[6] = {speed, speed+10, speed+25, speed+30, speed+40, speed+50};
	// if(move == 1){
		float rotate = 0;
		for(int i = 0; i<6; i++){
			// printf("i : %f \n", i);
			// bool display = true;
			speed = speed + 10*i;
			rotate = walls[i];
			speeds[i] = speeds[i]-(5*wallTime);
			// speeds[i] -= 10;
			glPushMatrix();
				glRotatef(rotate, 0, 1, 0);
				speed_wall(speeds[i], i);
			glPopMatrix();
		}


	// }
}

int main(int argc, char** argv)
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//########_CHARGER_IMAGES_###################
	GLuint texture[32];

	// Charger image menu
	texture[0] = loadTexture("doc/JEU_MENU_V0.jpg");

	// Charger image fin réussite
	texture[1] = loadTexture("doc/JEU_ECHEC.jpg");

	// Charger image fin réussite
	// texture[2] = loadTexture("doc/2k_earth_specular_map.tif");
	texture[2] = loadTexture("doc/POKEBALL_TEXTURE.jpg");

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	glPointSize(5.0);
	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();
		

		// Récupération du temps pour le mouvement des murs
		if(move == 1 && firstStart == 0){
			// paused = glfwGetTime()-paused;
        	currentTimeWalls = ((currentTimeWalls + startTime - pausedTime)/2);
		}else if(move == 0 && firstStop == 0){
			// currentTimeWalls = currentTimeWalls;
			pausedBegin = currentTimeWalls;
			firstStart = 1;
			firstStop = 1;
		}else if(move == 0 && firstStop == 1){
			pausedEnd = startTime;
		}else if(move == 1 && firstStart == 1){
			// currentTimeWalls = (pausedBegin + startTime)/2;
			currentTimeWalls = pausedBegin;
			startTime = pausedEnd - pausedBegin;
			pausedTime = pausedEnd - pausedBegin;
			firstStart = 0;
			firstStop = 0;
		}
		printf("currentTimeWalls %f \n", currentTimeWalls);

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();
		// drawFrame();

		/* Initial scenery setup */
		glEnable(GL_TEXTURE_2D); // Active la texture

		// Si le mode de jeu est à 0 : on est donc dans le menu
		if(mode == 0){ // page menu
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBegin(GL_POLYGON);
				glTexCoord3f(0,0,0);
				glVertex3f(-15.,0.,15.);
				
				glTexCoord3f(1,0,0);
				glVertex3f(15.,0.,15.);

				glTexCoord3f(1,1,0);
				glVertex3f(15.,0.,-15.);

				glTexCoord3f(0,1,0);
				glVertex3f(-15.,0.,-15.);
			glEnd();
		}

		if(mode == 1){
			//mur fond
			glPushMatrix();
				glColor3f(1,1,1); 
				glScalef(32,1,25);
				glTranslatef(0,68,0); //70 sur le PC normal
				drawSquare();
			glPopMatrix();
			//mur droit
			glPushMatrix();
				glColor3f(0,0,1); 
				glTranslatef(16,35,0);
				glRotatef(90,0,0,1);
				glScalef(75,1,25);
				drawSquare();
			glPopMatrix();
			//mur gauche
			glPushMatrix(); 
				glColor3f(0,0,1); 
				glTranslatef(-16,35,0);
				glRotatef(90,0,0,1);
				glScalef(75,1,25);
				drawSquare();
			glPopMatrix();
			//mur haut
			glPushMatrix();
				glColor3f(0,0.8,1); 
				glTranslatef(0,35,12.5);
				glRotatef(90,1,0,0);
				glRotatef(90,0,1,0);
				glScalef(75,1,32);
				drawSquare();
			glPopMatrix();
			//mur bas
			glPushMatrix();
				glColor3f(0,0.8,1); 
				glTranslatef(0,35,-12.5);
				glRotatef(90,1,0,0);
				glRotatef(90,0,1,0);
				glScalef(75,1,32);
				drawSquare();
			glPopMatrix();

			float speed = 50;
			float walls[6] = {0, 90, 180, 90, -90, 180};
			// if(!move){
				displayWalls(walls, speed, currentTimeWalls);
			// }else{
				// PausedTimeWalls = currentTimeWalls;
				// displayWalls(walls, speed, currentTimeWalls);
			// }
			// glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
			
            drawSphere();
			drawRacket();
		}

		if(mode == 2){ //page de fin
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glBegin(GL_POLYGON);
				glTexCoord3f(0,0,0);
				glVertex3f(-15.,0.,15.);

				glTexCoord3f(1,0,0);
				glVertex3f(15.,0.,15.);

				glTexCoord3f(1,1,0);
				glVertex3f(15.,0.,-15.);

				glTexCoord3f(0,1,0);
				glVertex3f(-15.,0.,-15.);
			glEnd();
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		/* Scene rendering */

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */
	}

	glfwTerminate();
	return 0;
}