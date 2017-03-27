
#include "Moving_Model.h"
#include "Main.h"

//int bombModId;

Moving_Model::Moving_Model(std::vector<glm::vec3> vertices_init, std::vector<glm::vec2> uvs_init, 
	std::vector<glm::vec3> normals_init, Texture *text,GLuint objectID)
:Model(vertices_init, uvs_init, normals_init,text,objectID){

}

void Moving_Model::set_speed(GLfloat setSpeedx,GLfloat setSpeedy,GLfloat setSpeedz) {
	speedx = setSpeedx;
	speedz = setSpeedz;
	speedy = setSpeedy;
}

void Moving_Model::set_direction_koefficents(GLfloat x, GLfloat y, GLfloat z){
	xKoff = x; 
	yKoff = y;
	zKoff = z;
}

void Moving_Model::update(int i){			//when i==1, this situation is to make a paowuxian, and vy should decrease by time.
	if (i == 1) 
		speedy = speedy - 0.001;

	posX += speedx*xKoff*get_time_update();
	posY += speedy*yKoff*get_time_update();
	posZ += speedz*zKoff*get_time_update();
	collisionBox->set_position(posX + getCamX(), posY + getCamY(), posZ + getCamZ());
	check_collision();
}

void Moving_Model::check_collision(){
	bool destroy = false;
	std::vector<Model*>* models = get_models();
	std::vector<Moving_Model*>* moving_models = get_moving_models();
	GLuint coll = 0, myPos=0;
	for (int i = 0; i < models->size(); i++){
		coll = models->at(i)->check_collision(collisionBox);

		if (id != models->at(i)->id){
			if (coll == 1 || coll == 3 || coll == 2){
				 if (models->at(i)->getObjectID() == models->at(i)->FLOOR) {//to check wheter the bomb fall on the floor
					if ((Bomb_situation == 1)||(Bomb_situation == 501)) {
						speedx = speedy = speedz = 0;
						printf("FLOOR collision");//Floor collision
						Bomb_situation ++;
					}
				}
			}
		}
	}
}

void Moving_Model::removeFromMovingModels(std::vector<Moving_Model*>* models, GLuint id){
	bool destroy = false;
	GLuint coll = 0, myPos = -1;
	for (int i = 0; i < models->size(); i++){
		if (id == models->at(i)->id)
			myPos = i;
	}
	printf("Remove");
	if (myPos != -1)
		models->erase(models->begin() + myPos);
}

void Moving_Model::check_ypos() {			//when bomb fire is underground, delete it
	std::vector<Model*>* models = get_models();
	if (posY < -6) {
		printf("FIRE removed");
		removeFromModels(models);
		removeFromMovingModels(get_moving_models(), id);
	}
}

void Moving_Model::Bomb_explosion(std::vector< glm::vec3 > block_vertices,std::vector< glm::vec2 > block_uvs,std::vector< glm::vec3 > block_normals)	//To display fire
{
	std::vector<Model*>* models = get_models();
	std::vector<Moving_Model*>* moving_models = get_moving_models();

	int ax = proposal(posX);		//To regulate the center of bomb fire. As the basic length of wall is 2*2, then the center should be the center of a 2*2 rectangular.
	int ay = get_pos_y()/2;
	int az = proposal(posZ);

	removeFromModels(models);
	removeFromMovingModels(moving_models, id);

	Moving_Model *triangle1 = (new Moving_Model(block_vertices, block_uvs, block_normals, textures[FIRE], triangle1->BOMBFIRE));     //Just to create 2 fire box.
	triangle1->setScale(4.0, 0.8, 0.8);//0.1 0.1 0.1
	triangle1->set_speed(0.2, 0.2, 0.2);
	triangle1->move(ax, ay, az, MOVE_IGNORE_ZERO);
	triangle1->set_direction_koefficents(0,-0.01, 0);

	Collision_Box *box = triangle1->get_collision_box();
	box->type = box->BOMBFIRE;
	models->insert(models->end(), triangle1);
	moving_models->insert(moving_models->end(), triangle1);

	Moving_Model *triangle2 = (new Moving_Model(block_vertices, block_uvs, block_normals, textures[FIRE], triangle2->BOMBFIRE));
	triangle2->setScale(0.8, 0.8, 4.0);//0.1 0.1 0.1
	triangle2->set_speed(0.2, 0.2, 0.2);
	triangle2->move(ax, ay, az, MOVE_IGNORE_ZERO);
	triangle2->set_direction_koefficents(0,-0.01, 0);

	Collision_Box *box2 = triangle2->get_collision_box();
	box2->type = box->BOMBFIRE;
	models->insert(models->end(), triangle2);
	moving_models->insert(moving_models->end(), triangle2);
}

void Moving_Model::Wall_eclipse(double length_x,double length_z) //To erase those broken walls and monsters
{
	int x = proposal(posX);							//To regulate the center that bomb falls in.
	int z = proposal(posZ);

	std::vector<Model*>* models = get_models();
	
	int i = 0;
	int N = models->size();
		
	while(i<N) {
		if (models->at(i)->getObjectID() == SOLID) {				//When the center is centain, it's possible to calculate whether each items lies in the bomb fire field.
			int other_x = proposal(models->at(i)->get_pos_x());		//Here we set 4 as bomb fire length.
			int other_z = proposal(models->at(i)->get_pos_z());
			if ((((other_x - x) <= 4) && ((other_x - x) >= -4) && (other_z == z))
				|| (((other_z - z) <= 4) && ((other_z - z) >= -4) && (other_x == x))) {
				stone_fly(other_x, other_z);
				models->erase(models->begin() + i);
				N--;
			}
			else
				i++;
		}
		else if (models->at(i)->getObjectID() == GHOST) {				//When the center is centain, it's possible to calculate whether each items lies in the bomb fire field.
			int other_x = proposal(models->at(i)->get_pos_x());		//Here we set 4 as bomb fire length.
			int other_z = proposal(models->at(i)->get_pos_z());
			if ((((other_x - x) <= 4) && ((other_x - x) >= -4) && (other_z <= z+2) && (other_z >= z - 2))
				|| (((other_z - z) <= 4) && ((other_z - z) >= -4) && (other_x <= x + 2) && (other_x >= x - 2))) {
				models->at(i)->alive = false;
				models->at(i)->move(models->at(i)->get_pos_x(), models->at(i)->get_pos_y() + 2, models->at(i)->get_pos_z(), MOVE_IGNORE_ZERO);
				models->at(i)->rotateY(angleY + 3.14);
				speedx = speedy = speedz = 0;
				models->at(i)->setAmbient(0.0f);
				models->at(i)->set_ignore_collision(true);
				models->at(i)->Bomb_situation = 1001;
				i++;
			}
			else
				i++;
		}
		else
			i++;
	}
}

int proposal(double x) {										//To propose a double number into a int number, which is the center of a 2*2 rectangular.
	int y = (int)(x + 0.5) > (int)x ? (int)x + 1 : (int)x;
	if (y % 4 == 1)
		y = y - 1;
	if (y % 4 == 3)
		y = y + 1;
	if (y % 4 == 2) {
		if ((x - y) > 0)
			y = y + 2;
		else
			y = y - 2;
	}
	return y;
}
