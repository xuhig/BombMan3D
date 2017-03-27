#include "Collision_Box.h"
class Model_Skeleton{
public:
	virtual void draw(void) = 0;
	virtual void move_collision_box() = 0;
	virtual void update(int i) = 0;			//This one is especially important, for it's the foundation of all models.
	virtual int check_collision(Collision_Box*) = 0;
	virtual void create_collision_box(void) = 0;
private:
};