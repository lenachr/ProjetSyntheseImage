#include "draw_scene.h"
#include "3D_tools.h"

void drawBalle(double posX, double posY, double posZ){
	glTranslatef(posX, -posY, posZ); 
    drawSphere();
}