#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "3D_tools.h"
#include "draw_scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "Main Menu";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */

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
			mode = 2;
			printf("Options \n");
		}
		// Clic sur options
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn02Xmin && Xpos < btn02Xmax && Ypos > btn02Ymin && Ypos < btn02Ymax){
			mode = 1;
			printf("Jouer ! \n");
		}
		// Clic sur quitter
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn03Xmin && Xpos < btn03Xmax && Ypos > btn03Ymin && Ypos < btn03Ymax){
			// On ferme la fenêtre
			printf("Quitter \n");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	// Si le mode de jeu est à 1 : on est donc dans le jeu
	if(mode == 1){
		
	}
	// Si le mode de jeu est à 2 ou 3 : on a fini le jeu
	if(mode == 2 || mode == 3){
		// Clic sur rejouer
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn02Xmin && Xpos < btn02Xmax && Ypos > btn02Ymin && Ypos < btn02Ymax){
			mode = 0;
			printf("Options \n");
		}
		// Clic sur quitter
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && Xpos > btn03Xmin && Xpos < btn03Xmax && Ypos > btn03Ymin && Ypos < btn03Ymax){
			// On ferme la fenêtre
			printf("Quitter \n");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}	
	}
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

int erreur(unsigned char *image){
	if(image != NULL){
		printf("reussi \n");
	}else{
		printf("rate \n");
		return -1;

	}
	return 0;
}

// GLuint loadTexture(const char* image, char imageName){
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	return texture;
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
	texture[1] = loadTexture("doc/JEU_ECHEC.png");

	// Charger image fin réussite
	texture[2] = loadTexture("doc/POKEBALL_TEXTURE.png");

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

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();
		// drawFrame();

		/* Initial scenery setup */

		// Si le mode de jeu est à 0 : on est donc dans le menu
		if(mode == 0){
			glEnable(GL_TEXTURE_2D); // Active la texture
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
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            drawSphere();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
			drawRacket();
		}

		if(mode == 2){
			glEnable(GL_TEXTURE_2D); // Active la texture
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
	// glDeleteTextures(2, &texture);

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
