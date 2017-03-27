
#include "Ghost.h"
#include "Main.h"
#include "Math_Methods.h"
#include "Line_Of_Sight_Object.h"

Ghost::Ghost(std::vector<glm::vec3> vertices_init, std::vector<glm::vec2> uvs_init, std::vector<glm::vec3> normals_init, Texture *text)
:Moving_Model(vertices_init, uvs_init, normals_init, text,GHOST){
}

void Ghost::update(int i){						//In fact this i is useless. But since we use update(int i) before and it's override,
	GLfloat xDiff = posX + getCamX();			//it should have some i I suppose.
	GLfloat yDiff = posY + getCamY();
	GLfloat zDiff = posZ + getCamZ();
	GLfloat distance = calc_distance(xDiff, yDiff, 0, 0, zDiff, 0);
	speedx = speedy = speedz = maxspeed;
	xDiff *= -1; zDiff *= -1; //yDiff *= -1;
	GLfloat angle;
	angle = calc_angle(xDiff, 0, zDiff, 0);
	xKoff = cos(angle);
	yKoff = 0;
	zKoff = sin(angle);

	if (distance < active_distance&&alive){
	if (counter >= 30){
		
			printf("Create LOS");
			//los_object->removeFromModels(get_models());
			free(los_object);
			
			los_object = create_los_object(in_sight);
			los_object->setScale(0.1, 0.1, 0.1);
			los_object->set_speed(0.2,0.2,0.2);
			los_object->setInSight(in_sight);
			los_object->move(posX + getCamX(), posY + getCamY() + 2, posZ + getCamZ(), MOVE_IGNORE_ZERO);
			los_object->set_koeffs(xKoff, 0, zKoff);

			counter = 0;
		
	}
	los_object->update(0);
	in_sight = los_object->get_in_sight();
	if (los_object != NULL && in_sight){

			rotateX(-angle - 3.14 / 10 * 9.5);
			posX += speedx*xKoff*get_time_update();
			posY += speedy*yKoff*get_time_update();
			posZ += speedz*zKoff*get_time_update();

			collisionBox->set_position(posX + getCamX(), posY + getCamY(), posZ + getCamZ());

			if (distance < 3){

				exit(1);
			}
		}
		else{
			speedx = speedy = speedz = 0;
		}
	}
		counter++;
}