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
}

void drawArm() {
}

void drawPan() {
}
