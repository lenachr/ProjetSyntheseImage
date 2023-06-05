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

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "Main Menu";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

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

/* Variables Globales */

// Déclaration des variables de la raquette
float racketX = 2;
float racketZ = 2;
float racketWidth = 1.5;
float racketHeight = 1.5;

// Pour permettre d'avancer en appuyant sur la touche flèche du haut
int move = 0;
int firstStart = 0;
int firstStop = 0;
double currentTimeWalls = 1;
double pausedBegin = 1;
double pausedEnd = 0;
double pausedTime = 0;

// Pour savoir si la balle est tenue par la raquette ou pas
int holdBall = 1;

int firstGame = 2;
// Initialise le nombre de vies
int numberLifes = 5;

// Savoir si on possède le bonus colle
int getBonusGlue = 0;
int resetGlue = 0;

// Pour gérer les déplacements de la balle
double posX = 0;
double posY = 0;
double posZ = 0;
double XDirection = 0;
double YDirection = 1;
double ZDirection = 0;

//Gestions des différentes étapes : menu = 0, jeu = 1, fenêtre de fin echec = 2, fenêtre de fin reussite = 3
// On pourra juste avoir une image différente pour les deux fins avec les boutons et les fonctionnalités identiques
int mode = 0;

// Créer un aléatoire pour les deux types de bonus
int getRandomZeroOrOne() {
    return rand() % 2;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	// Si le mode de jeu est à 1, 2 ou 3 : on est donc dans le jeu
	if(mode == 1 || mode == 2 || mode == 3){
		// Clic pour lancer la balle
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
			holdBall = 0;
		}

		// Gestion du "double clic"
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && firstGame != 1){
			firstGame = 1;
		}
	}
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS && GLFW_KEY_UP){
		// On ne peut avancer que si on a lancé la balle
		if(holdBall == 0){
			move = 1;
		}
	}else{			
		move == 0;
	}
	if(action == GLFW_PRESS){
		if (mode == 0) {
			switch(key) {
				case GLFW_KEY_C :
					mode = 1;
					break;
				case GLFW_KEY_V :
					mode = 2;
					break;
				case GLFW_KEY_B :
					mode = 3;
					break;
				case GLFW_KEY_N :
					mode = 5;
					break;
				case GLFW_KEY_X :
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
				default: fprintf(stdout,"Touche non gérée (%d)\n",key);	
			}
		}

		if (mode == 4) {
			switch(key) {
				case GLFW_KEY_J :
					mode = 0;
					break;
				case GLFW_KEY_X :
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;	
				default: fprintf(stdout,"Touche non gérée (%d)\n",key);
			}
		}

		if (mode == 5) {
			switch(key) {
				case GLFW_KEY_J :
					mode = 0;
					break;
				case GLFW_KEY_X :
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;	
				default: fprintf(stdout,"Touche non gérée (%d)\n",key);
			}
		}
	}
	if (action == GLFW_RELEASE){
		switch(key) {
			case GLFW_KEY_UP :
				move = 0;
				break;	
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

//gère le calcul de la position de la souris/raquette
void onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    float normX = (2.0f * xpos) / WINDOW_WIDTH - 1.0f;
    float newRacketX = normX * 10.0f; // Position horizontale de la raquette
    float normY = (2.0f * ypos) / WINDOW_HEIGHT - 1.0f;
    float newRacketZ = -normY * 10.0f; // Position verticale de la raquette

	// Vérifier les limites de la zone de jeu
    if (newRacketX - racketWidth >= -9.0f && newRacketX + racketWidth <= 9.0f)
        racketX = newRacketX;

    if (newRacketZ - racketHeight >= -7.0f && newRacketZ + racketHeight <= 7.0f)
        racketZ = newRacketZ;

	if(holdBall == 1 && firstGame == 2){
        posX = racketX*2;
        posZ = 1-racketZ*2;
    }
	
	if(holdBall == 1 && firstGame == 0){
	    posX = racketX*2;
	    posZ = 1-racketZ*2;
	}

	if(holdBall == 1 && getBonusGlue == 1){
	    posX = racketX*2;
	    posZ = 1-racketZ*2;
	}
}

void erreur(unsigned char *image){
	if(image != NULL){
		// printf("reussi \n");
	}else{
		// printf("rate \n");

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	return texture;
}

// Fait avancer les obstacles walls
void speed_wall(float speedy, int i, float wallX, float wallZ){
		glPushMatrix();
			float color = 1-(speedy/15.);
			glPushMatrix();
			glColor3f(color,color,color); 
			glPopMatrix();
			if(wallZ == 0){
				glScalef(12.5,7,25);
			}else{
				glScalef(16,7,32);
			}
			glTranslatef(-0.5,speedy,0); 
			speedy = speedy * 5.62;

			// On teste s'il y a une collision de l'obstacle avec la balle OU si la raquette est devant un mur
			if(wallZ == 0){
				if(	posX + 0.5 >= wallX - 6.25 && posX - 0.5 <= wallX + 6.25 &&
        			posY + 0.5 >= speedy - 2 && posY - 0.5 <= speedy+ 2)
				{
					YDirection = -YDirection;

				}
				// Empêcher les walls (obstacles) d'avancer :
        		if (speedy-2 <= 3 && speedy > -1
					&& racketX + 1 <= wallX + 6.25
					&& racketX - 1 >= wallX - 6.25)
					{
					// Ne pas laisser les walls (obstacles) avancer
					move = 0;
				}
			}
			if(wallX == 0){
				if( posY + 0.5 >= speedy - 2 && posY - 0.5 <= speedy + 2 &&
					posZ + 0.5 >= wallZ - 8 && posZ - 0.5 <= wallZ + 8)
				{
					YDirection = -YDirection;

				}
				// Empêcher les walls (obstacles) d'avancer :
        		if (speedy-2 <= 3 && speedy > -1
					&& racketZ + 1 <= wallZ + 8
					&& racketZ - 1 >= wallZ - 8)
					{
					// Ne pas laisser les walls (obstacles) avancer
					move = 0;
				}
			}

			drawSquare();
		glPopMatrix();
}

// Fait avancer les bonus
void speed_bonusLife(float speedy, int i, double bonusX, double bonusZ, int type){
		glPushMatrix();
			float color = 1-(speedy/15.);
			if(type == 1){
				glPushMatrix();
				glColor3f(1.,color,color); 
				glPopMatrix();
			}else{
				glPushMatrix();
				glColor3f(0., color, color); 
				glPopMatrix();
			}
				glScalef(3,3,3);
			glTranslatef(bonusX, speedy, bonusZ); 

			speedy = speedy * 5.62;
			drawCone();
		glPopMatrix();
}

// Gère le tableau d'obstacles
void displayWalls(float* walls, float speed, double wallTime){
	// Niveau 1
	if(mode == 1){
		float speeds[6] = {speed, speed+10, speed+25, speed+30, speed+40, speed+50};
		float wallX = 0;
		float wallZ = 0;

		// {0, 90, 180, 90, -90, 180}
		for(int i = 0; i<6; i++){
			float rotate = 0;
			speed = speed + 10*i;
			rotate = walls[i];
			// On teste la rotation pour donner la bonne valeur à wallX et wallZ
			if(rotate == 0){
				wallX = -5;
				wallZ = 0;
			}
			if(rotate == -90){
				wallX = 0;
				wallZ = -5;
			}
			if(rotate == 90){
				wallX = 0;
				wallZ = 5;
			}
			if(rotate == 180){
				wallX = 5;
				wallZ = 0;
			}
			speeds[i] = speeds[i]-(3*wallTime);
			
			glPushMatrix();
				glRotatef(rotate, 0, 1, 0);
				speed_wall(speeds[i], i, wallX, wallZ);
			glPopMatrix();
		}
	}

	// Niveau 2
	if(mode == 2){
		float speeds[12] = {speed, speed+10, speed+25, speed+30, speed+40, speed+50, speed, speed+55, speed+65, speed+75, speed+90, speed+100};
		float wallX = 0;
		float wallZ = 0;

		for(int i = 0; i<12; i++){
			float rotate = 0;
			speed = speed + 10*i;
			rotate = walls[i];
			// On teste la rotation pour donner la bonne valeur à wallX et wallZ
			if(rotate == 0){
				wallX = -5;
				wallZ = 0;
			}
			if(rotate == -90){
				wallX = 0;
				wallZ = -5;
			}
			if(rotate == 90){
				wallX = 0;
				wallZ = 5;
			}
			if(rotate == 180){
				wallX = 5;
				wallZ = 0;
			}
			speeds[i] = speeds[i]-(3*wallTime);
			
			glPushMatrix();
				glRotatef(rotate, 0, 1, 0);
				speed_wall(speeds[i], i, wallX, wallZ);
			glPopMatrix();
		}
	}

	// niveau 3
	if(mode == 3){
		float speeds[18] = {speed, speed+10, speed+25, speed+30, speed+40, speed+50, speed, speed+55, speed+65, speed+75, speed+90, speed+100, speed+105, speed+120, speed+130, speed+135, speed+140, speed+150};
		float wallX = 0;
		float wallZ = 0;

		for(int i = 0; i<18; i++){
			float rotate = 0;
			speed = speed + 10*i;
			rotate = walls[i];
			// On teste la rotation pour donner la bonne valeur à wallX et wallZ
			if(rotate == 0){
				wallX = -5;
				wallZ = 0;
			}
			if(rotate == -90){
				wallX = 0;
				wallZ = -5;
			}
			if(rotate == 90){
				wallX = 0;
				wallZ = 5;
			}
			if(rotate == 180){
				wallX = 5;
				wallZ = 0;
			}
			speeds[i] = speeds[i]-(3*wallTime);
			
			glPushMatrix();
				glRotatef(rotate, 0, 1, 0);
				speed_wall(speeds[i], i, wallX, wallZ);
			glPopMatrix();
		}
	}	
}

// Gère le tableau de bonus
void displayBonus(float* bonuss, float speed, double wallTime, int* collected, int type){
	float speeds[3] = {speed+15, speed+40, speed+70};
		double bonusX = 0;
		double bonusZ = 0;

		for(int i = 0; i<3; i++){
			float rotate = 0;
			speed = speed + 10*i;
			rotate = bonuss[i];
			if(rotate == 0){
				bonusX = 3;
				bonusZ = 1;
			}
			if(rotate == -90){
				bonusX = -2;
				bonusZ = 1;
			}
			if(rotate == 90){
				bonusX = 2;
				bonusZ = -1;
			}
			if(rotate == 180){
				bonusX = -1;
				bonusZ = -1;
			}
			speeds[i] = speeds[i]-(5*wallTime);
			glPushMatrix();
			if(collected[i] == 0){
				speed_bonusLife(speeds[i], i, bonusX, bonusZ, type);
				collected[i] = collisionsBonus(speeds[i], bonusX, bonusZ, collected[i], type);
				if((collected[i]) == 0){
				}
			}
			glPopMatrix();
		}
}

// Savoir quand la raquette a récupéré un bonus et appelle la fonction bonus correspondante
void collisionsBonus(double bonusY, double bonusX, double bonusZ, int* collected, int type){
	// On teste s'il y a une collision de la raquette avec le bonus
	if(	racketX + racketWidth/2 >= bonusX - 1.5 && racketX - racketWidth/2 <= bonusX + 1.5 &&
		bonusY <= 0.1 && bonusY >= 0 &&
		racketZ + racketHeight/2 >= bonusZ - 1.5 && racketZ - racketHeight/2 <= bonusZ + 1.5)
	{
		collected = 1;
		if(type == 1){
			actionBonusLife();
		}else{
			actionBonusGlue();
		}
	}
}

// Quand on récupère un bonus avec la raquette
void actionBonusLife(){
	numberLifes += 1;
}

void actionBonusGlue(){
	getBonusGlue = 1;
}

// Gère les collisions de la balle
void bounceBall(){
	double speed = 0.5;
	// Si collision avec les bords du couloir en X (donc à droite et à gauche)
	if(posX < -15 || posX > 15){
		XDirection = -XDirection;	
	}
	// Si collision avec la fin du couloir
	if(posY > 70){
		YDirection = -YDirection;
	}
	// Si collision avec les bords du couloir en Z (donc en haut et en bas)
	if(posZ < -11.5 || posZ > 11.5){
		ZDirection = -ZDirection;	
	}

	if(posY+0.5 < -2){
		ballMissed();
	}
		posX = (posX + XDirection*(speed/2));
		posY = (posY + YDirection*speed);
		posZ = (posZ + ZDirection*(speed/2));
}

// Quand on perd la balle
void ballMissed(){
	// On n'est plus au premier jeu
	firstGame = 0;

	// On perd une vie
	numberLifes -= 1;
	
	// On arrête le mouvement des obstacles
	// NOTE : pourquoi est-ce qu'on doit relacher la flèche ? (car les murs sont censés s'arrêter mais ils ne le font pas, impossible de trouver la solution)
	move == 0;

	// On remet en place les variables globales de déplacement de la balle
	posX = racketX;
	posY = 1;
	posZ = racketZ;
	XDirection = 0;
	YDirection = 1;
	ZDirection = 0;
	// Si on a toujours des vies
	if(numberLifes != 0){
	// On met la variable holdBall à 1 pour pouvoir la renvoyer
		holdBall = 1;
		// On perd le bonus potentiellement gagné précedemment
		getBonusGlue = 0;
		resetGlue = 0;		
	}else{
		// Si on n'a plus de vies on passe le mode de jeu à 2 ce qui arrêtre le jeu
		mode = 4;
		// Si on veut rejouer le jeu, on remet les 5 vies originales
		numberLifes = 5;
		// On repasse en premier jeu
		firstGame = 2;
		// On perd le bonus potentiellement gagné précedemment
		getBonusGlue = 0;
		resetGlue = 0;
	}
	printf("%d \n",numberLifes);
}

//gère la raquette
void drawRacket(){

            glTranslatef(racketX, -1.5, racketZ );
            // glTranslatef(racketX,-10.0, racketZ );
            glScalef(racketWidth, 1.0, racketHeight);

            glBegin(GL_LINES);
			glPushMatrix();
			// Idée d'un bonus qui permet de plus facilement savoir où rattraper la balle
			// if(	(posX/2 - 0.5) <= racketX + racketWidth && (posX/2 + 0.5) >= racketX - racketWidth &&
			// 	(posZ/2 - 0.5) <= racketZ + racketHeight && (posZ/2 + 0.5) >= racketZ - racketHeight)
			// {
            // 	glColor3f(0,1,0);
			// }else{
            // 	glColor3f(1,0,0);
			// }
            glColor3f(1,1,1);
			glPopMatrix();
            glVertex3f(-racketWidth, 0, racketHeight);  // Coin supérieur gauche
            glVertex3f(racketWidth, 0, racketHeight);   // Coin supérieur droit
            glVertex3f(racketWidth, 0, racketHeight);   // Coin supérieur droit
            glVertex3f(racketWidth, 0, -racketHeight);  // Coin inférieur droit
            glVertex3f(racketWidth, 0, -racketHeight);  // Coin inférieur droit
            glVertex3f(-racketWidth, 0, -racketHeight); // Coin inférieur gauche
            glVertex3f(-racketWidth, 0, -racketHeight); // Coin inférieur gauche
            glVertex3f(-racketWidth, 0, racketHeight);  // Coin supérieur gauche
            glEnd();

			bounceRacket();

}

void bounceRacket(){
	// En cas de collision avec la balle
	if(	(posX/2 - 0.5) <= racketX + racketWidth && (posX/2 + 0.5) >= racketX - racketWidth &&
		posY - 0.5 <= 0 &&
		(posZ/2 - 0.5) <= racketZ + racketHeight && (posZ/2 + 0.5) >= racketZ - racketHeight)
	{
		// Si on a récupéré le bonus colle la balle ne repart pas directement
		if(getBonusGlue == 1){
			// On remet en place les variables globales de déplacement de la balle
			move = 0;
			holdBall = 1;
			posX = racketX;
			posY = 1;
			posZ = racketZ;
			XDirection = 0;
			YDirection = 1;
			ZDirection = 0;
			resetGlue = 0;
		}else{
			XDirection = 1;
			ZDirection = 1;
			double distanceX = posX - racketX;
			double distanceZ = posZ + racketZ;
			XDirection = distanceX*XDirection;
			YDirection = -YDirection;
			ZDirection = distanceZ*ZDirection;
		}
	}
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
	glfwSetCursorPosCallback(window, onMouseMove);

	//########_CHARGER_IMAGES_###################
	GLuint texture[32];

	// Charger image menu
	texture[0] = loadTexture("doc/menu_depart.jpg");

	// Charger image fin réussite
	texture[1] = loadTexture("doc/JEU_ECHEC.jpg");

	// Charger image fin réussite
	texture[2] = loadTexture("doc/POKEBALL_TEXTURE.jpg");

	// Charger image fin réussite
	texture[3] = loadTexture("doc/menu_gain.jpg");

	glfwSetWindowSizeCallback(window,onWindowResized);
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glPointSize(5.0);
	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		// Récupération du temps pour le mouvement des murs
		if(move == 1 && firstStart == 0){
        	currentTimeWalls = ((currentTimeWalls + startTime - pausedTime)/2);
		}else if(move == 0 && firstStop == 0){
			pausedBegin = currentTimeWalls;
			firstStart = 1;
			firstStop = 1;
		}else if(move == 0 && firstStop == 1){
			pausedEnd = startTime;
		}else if(move == 1 && firstStart == 1){
			currentTimeWalls = pausedBegin;
			startTime = pausedEnd - pausedBegin;
			pausedTime = pausedEnd - pausedBegin;
			firstStart = 0;
			firstStop = 0;
		}

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		/* Poll for and process events */
		glfwPollEvents();

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

			//dessin de la raquette
			glPushMatrix();
            	glTranslatef(racketX, 0.0, racketZ);
            	drawRacket();
            glPopMatrix();

			//mur fond
			glPushMatrix();
				glColor3f(0,0,0); 
				glScalef(32,1,25);
				glTranslatef(0,68,0);
				drawSquare();
			glPopMatrix();

			//mur droit			
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,0,color); 
					glTranslatef(16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}
					
			//mur gauche
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,0,color); 
					glTranslatef(-16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}

			//mur haut
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,0,color); 
					glTranslatef(0,i,12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}

			//mur bas
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,0,color); 
					glTranslatef(0,i,-12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}

			float speed = 18;

			// Gestion obstacles
			float wallsLvl01[6] = {0, 90, 180, 90, -90, 180};
                displayWalls(wallsLvl01, speed, currentTimeWalls);
			
            	glBindTexture(GL_TEXTURE_2D, texture[2]);
			// glPushMatrix();
				if(holdBall == 0){
					bounceBall();
					glBindTexture(GL_TEXTURE_2D, texture[2]);
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
						drawBalle((posX), (posY+0.1), (posZ));
					glPopMatrix();
				}else{
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
            			// drawSphere();
						if(firstGame == 1){
							drawBalle((0), (0.1), (0));
						}else{
							drawBalle((posX), (0.1), (posZ));
						}
					glPopMatrix();
				}
			// glPopMatrix();

			// Gestion Bonus
			float bonusLife[3] = {0, 90, 180};
			float bonusGlue[3] = {180, -90, 180};
			int collectedLife[3] = {0, 0, 0};
			int collectedGlue[3] = {0, 0, 0};
			int colorLife = 1;
			int colorGlue = 0;
			// On peut reprendre le même temps que pour les obstacles
				displayBonus(bonusLife, 18, currentTimeWalls, collectedLife, colorLife);
				displayBonus(bonusGlue, 12, currentTimeWalls, collectedGlue, colorGlue);

			// glEnable(GL_TEXTURE_2D);
		}

		if(mode == 2){

			glPushMatrix();
            	glTranslatef(racketX, 0.0, racketZ);
            	drawRacket(racketX, 0.0, racketZ);
            glPopMatrix();
			//mur fond
			glPushMatrix();
				glColor3f(0,0,0); 
				glScalef(32,1,25);
				glTranslatef(0,68,0); //70 sur le PC normal
				drawSquare();
			glPopMatrix();
			//mur droit			
            
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,color,0); 
					glTranslatef(16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}
					
			//mur gauche
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,color,0); 
					glTranslatef(-16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}

			//mur haut
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,color,0);
					glTranslatef(0,i,12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}


			//mur bas
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(0,color,0);
					glTranslatef(0,i,-12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}

			float speed = 18;

			// Gestion obstacles
			float wallsLvl02[12] = {180, 90, 0, 90, 180, 90, -90, 180, -90, 180};
				displayWalls(wallsLvl02, speed, currentTimeWalls);
            	glBindTexture(GL_TEXTURE_2D, texture[2]);
			// glPushMatrix();
				if(holdBall == 0){
					bounceBall();
					glBindTexture(GL_TEXTURE_2D, texture[2]);
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
						drawBalle((posX), (posY+0.1), (posZ));
					glPopMatrix();
				}else{
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
						if(firstGame == 1){
							drawBalle((0), (0.1), (0));
						}else{
							drawBalle((posX), (0.1), (posZ));
						}
					glPopMatrix();
				}
			// glPopMatrix();

			// Gestion Bonus
			float bonusLife[3] = {0, 90, 180};
			float bonusGlue[3] = {180, -90, 180};
			int collectedLife[3] = {0, 0, 0};
			int collectedGlue[3] = {0, 0, 0};
			int colorLife = 1;
			int colorGlue = 0;
			// On peut reprendre le même temps que pour les obstacles
				displayBonus(bonusLife, 18, currentTimeWalls, collectedLife, colorLife);
				displayBonus(bonusGlue, 12, currentTimeWalls, collectedGlue, colorGlue);

			// glEnable(GL_TEXTURE_2D);
		}

		if(mode == 3){

			glPushMatrix();
            	glTranslatef(racketX, 0.0, racketZ);
            	drawRacket(racketX, 0.0, racketZ);
            glPopMatrix();
			//mur fond
			glPushMatrix();
				glColor3f(0,0,0); 
				glScalef(32,1,25);
				glTranslatef(0,68,0); //70 sur le PC normal
				drawSquare();
			glPopMatrix();
			//mur droit			
            
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(color,0,0); 
					glTranslatef(16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}
					
			//mur gauche
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(color,0,0); 
					glTranslatef(-16,i,0);
					glRotatef(90,0,0,1);
					glScalef(5,1,25);
					drawSquare();
				glPopMatrix();		
			}

			//mur haut
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(color,0,0);
					glTranslatef(0,i,12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}


			//mur bas
			for(float i=75;i>-1;i-=5){
				float color = 1-i/75;
				glPushMatrix();
					glColor3f(color,0,0); 
					glTranslatef(0,i,-12.5);
					glRotatef(90,1,0,0);
					glRotatef(90,0,1,0);
					glScalef(5,1,32);
					drawSquare();
				glPopMatrix();		
			}

			float speed = 18;

			// Gestion obstacles
			float wallsLvl03[18] = {-90, 180, -90, 180, 0, 90, 180, 90, -90, 180, 0, -90, 180, 90, -90, 180};
                displayWalls(wallsLvl03, speed, currentTimeWalls);
			
            	glBindTexture(GL_TEXTURE_2D, texture[2]);
			// glPushMatrix();
				if(holdBall == 0){
					bounceBall();
					glBindTexture(GL_TEXTURE_2D, texture[2]);
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
						drawBalle((posX), (posY+0.1), (posZ));
					glPopMatrix();
				}else{
					glPushMatrix();
						glColor3f(1,1,1); 
						glRotatef(180,0,0,1);
						glRotatef(180,0,1,0);
            			// drawSphere();
						if(firstGame == 1){
							drawBalle((0), (0.1), (0));
						}else{
							drawBalle((posX), (0.1), (posZ));
						}
					glPopMatrix();
				}
			// glPopMatrix();

			// Gestion Bonus
			float bonusLife[3] = {0, 90, 180};
			float bonusGlue[3] = {180, -90, 180};
			int collectedLife[3] = {0, 0, 0};
			int collectedGlue[3] = {0, 0, 0};
			int colorLife = 1;
			int colorGlue = 0;
			// On peut reprendre le même temps que pour les obstacles
				displayBonus(bonusLife, 18, currentTimeWalls, collectedLife, colorLife);
				displayBonus(bonusGlue, 12, currentTimeWalls, collectedGlue, colorGlue);

			// glEnable(GL_TEXTURE_2D);
		}

		if(mode == 4){ //page de fin
			glColor3f(1,1,1); 
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

		if(mode == 5){ //page de fin
			glColor3f(1,1,1); 
			glBindTexture(GL_TEXTURE_2D, texture[3]);
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

		// /* Poll for and process events */
		// glfwPollEvents();

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