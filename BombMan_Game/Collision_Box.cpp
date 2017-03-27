#include "Collision_Box.h"
#include <iostream>
#include "Main.h"
Collision_Box::Collision_Box(GLfloat xleft, GLfloat xright, GLfloat yup, GLfloat ydown, GLfloat zin, GLfloat zout){
	xLeft = xleft;
	xRight = xright;
	yUp= yup;
	yDown = ydown;
	zIn = zin;
	zOut = zout;
	width = abs(xLeft - xRight);
	height = abs(yUp - yDown);
	depth = abs(zIn - zOut);
	xPos = 0; 
	yPos = 0;
	zPos = 0;
	xOffset = 0;
	yOffset = 0;
	zOffset = 0;
}

void Collision_Box::set_position(GLfloat x, GLfloat y, GLfloat z){
	xPos = x + xOffset;
	yPos = y + yOffset;
	zPos = z + zOffset;
}

void Collision_Box::set_offset(GLfloat x, GLfloat y, GLfloat z){
	xOffset = x;
	yOffset = y;
	zOffset = z;
}
int Collision_Box::check_collision(Collision_Box* otherBox){
	if (type == BULLET && otherBox->type == CAMERA || type == CAMERA && otherBox->type == BULLET){
		return 0;
 	}
	if ((type == BOMBFIRE && otherBox->type != GHOST)||(type != GHOST && otherBox->type == BOMBFIRE)) {//changegggggggggggg
		return NO_COLLISION;
	}
	if (-(width / 2) + xPos< otherBox->xPos + (otherBox->width / 2)
		&& xPos + (width / 2) > otherBox->xPos - (otherBox->width / 2)
		&& -(depth / 2) + zPos < otherBox->zPos + (otherBox->depth / 2)
		&& zPos + (depth / 2) > otherBox->zPos - (otherBox->depth / 2)){
		
		if (yPos + 1 + height / 2 < otherBox->yPos - otherBox->height / 2){

			if (abs((yPos + 1 + height / 2) - (otherBox->yPos - otherBox->height / 2))< 0.6)//0.6
				return ONTOP;
			else
				return NO_COLLISION;
		}
		else if (yPos - height / 2 > otherBox->yPos + otherBox->height / 2){
			if (abs((yPos - height / 2) - (otherBox->yPos + otherBox->height / 2)) <0.6){//0.6
				return COLLBELOW;
			}
			else
			return NO_COLLISION;
		}
		else{

			return COLLISION;
		}
	}
		return 0;
}

void Collision_Box::rotateX(GLdouble angle){

	GLfloat tempXLeft, tempXRight, tempZOut, tempZIn;

	tempXLeft = xLeft*cos(angle) + sin(angle)*zIn;
	tempXRight = xRight*cos(angle) + sin(angle)*zOut;

	tempZOut = xRight*(-sin(angle)) + cos(angle)*zOut;
	tempZIn = xLeft*(-sin(angle)) + cos(angle)*zIn;

	xLeft = tempXLeft;
	xRight = tempXRight;
	zOut = tempZOut;
	zIn = tempZIn;
	width = abs(xLeft - xRight);
	depth = abs(zIn - zOut);
	printf("\n\n\n xLeft %f, xRight %f, zOut %f, Zin %f \n\n\n",xLeft,xRight,zOut,zIn);
}


void Collision_Box::rotateY(GLdouble angle){			//Just to turn the monster over

	xLeft = -xLeft;
	xRight = -xRight;
	yUp = -yUp;
	yDown = -yDown;
	width = abs(xLeft - xRight);
	height = abs(yUp - yDown);

}
