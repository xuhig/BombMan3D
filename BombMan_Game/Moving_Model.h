#pragma once
#ifndef MOVING_MODEL_H
#define __Moving_Model_H__
#include "Model.h"
int proposal(double x);
class Moving_Model : public Model {

public:
	using Model::Model;
	Moving_Model(std::vector<glm::vec3> vertices_init, std::vector<glm::vec2> uvs_init, std::vector<glm::vec3> normals_init, Texture *text, GLuint objectId);
	void set_speed(GLfloat setSpeedx, GLfloat setSpeedy, GLfloat setSpeedz);
	void set_direction_koefficents(GLfloat koffX, GLfloat koffY, GLfloat koffZ);
	void Moving_Model::removeFromMovingModels(std::vector<Moving_Model*>* models, GLuint id);
	//GLuint Bomb_situation = 0;       //Now move it to Model.

	void Moving_Model::update(int i);
	void Moving_Model::check_collision(void);
	void Moving_Model::check_ypos(void);
	void Moving_Model::Bomb_explosion(std::vector< glm::vec3 > block_vertices, std::vector< glm::vec2 > block_uvs, std::vector< glm::vec3 > block_normals);
	void Moving_Model::Wall_eclipse(double length_x, double length_z);
protected:

	GLfloat speedx, speedy, speedz, xKoff, yKoff, zKoff;
};
#endif