#include "draw_scene.h"
#include "3D_tools.h"

void drawRacket(){
	glBegin(GL_LINES);
			// Première ligne horizontale
			glColor3f(1,1,1);
			glVertex3f(-2,0,2);
			glVertex3f(2,0,2);

			// Première ligne verticale
			glColor3f(1,1,1);
			glVertex3f(2,0,2);
			glVertex3f(2,0,-2);

			// Deuxième ligne horizontale
			glColor3f(1,1,1);
			glVertex3f(2,0,-2);
			glVertex3f(-2,0,-2);

			// Deuxième ligne verticale
			glColor3f(1,1,1);
			glVertex3f(-2,0,-2);
			glVertex3f(-2,0,2);

}

void drawFrame(){
    glBegin(GL_LINES);
            // Axe Rouge X
			glColor3f(1.0,0.0,0.0);
			glVertex3f(-10.0,0.0,0.0);
			glVertex3f(10.0,0.0,0.0);

            // Axe vert Y
			glColor3f(0.0,1.0,0.0);
			glVertex3f(0.0,-10.0,0.0);
			glVertex3f(0.0,10.0,0.0);

            // Axe Bleu Z
			glColor3f(0.0,0.0,1.0);
			glVertex3f(0.0,0.0,-10.0);
			glVertex3f(0.0,0.0,10.0);
		glEnd();
}

void drawBase() {
}

void drawArm() {
}

void drawPan() {
}
