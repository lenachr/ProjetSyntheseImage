#include "draw_scene.h"
#include "3D_tools.h"

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
	glColor3f(0.92,0.812,0.20);
	glPushMatrix();
		glScalef(2., 2., 10.);
		drawCone();
	glPopMatrix();
	glPushMatrix();
		glScalef(3., 3., 1.);
		drawCircle();
	glPopMatrix();
}

void drawArm() {
	// Couleur
	glColor3f(0.96,0.64,0.26);

	// Sphère
	glPushMatrix();
		glScalef(1.6, 1.6, 1.6);
		drawSphere();
	glPopMatrix();

	// Cônes
	glPushMatrix();
		glScalef(1., 10., 1.);
		glPushMatrix();
			glRotatef(-90, 1., 0., 0.);
			drawCone();
		glPopMatrix();
		glPushMatrix();
			glRotatef(90, 1., 0., 0.);
			drawCone();
		glPopMatrix();
	glPopMatrix();
}

void fils(){
	// Fils
	glPushMatrix();
		glColor3f(1.,0.1,0.1);
		// glTranslatef(0., -10., 0.);
		glBegin(GL_LINES);
		// corde 01
		glVertex3f(0, 0, 5);
		glVertex3f(0, -3, 0);

		// corde 02
		glVertex3f(0, 0, 5);
		glVertex3f(0, 3, 0);
		
		// corde 03
		glVertex3f(0, 0, 5);
		glVertex3f(-3, 0, 0);

		// corde 03
		glVertex3f(0, 0, 5);
		glVertex3f(3, 0, 0);
	glEnd();
	glPopMatrix();
}

void drawPan() {
	// Plateaux
	glPushMatrix();
		// fils();
		glPushMatrix();
			glTranslatef(0., 10, 0.);
			fils();
			glColor3f(0.35,0.45,0.);
			glScalef(3., 3., 1.);
			drawCircle();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0., -10, 0.);
			fils();
			glColor3f(0.35,0.45,0.);
			glScalef(3., 3., 1.);
			drawCircle();
		glPopMatrix();
	glPopMatrix();
}
