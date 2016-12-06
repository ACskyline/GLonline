#include "Box.h"
#include "Wall.h"
#define CBOFFSET 0.2
Box::Box()
{
	colliderType = COL2D_BOX;
	left = 0.1;
	up = 0.1;
	color = Color(0.1, 0.2, 0.3);
	life = -1;
}


Box::~Box()
{
}

Box::Box(float x_pos, float y_pos, float _speed, float x_dir, float y_dir, float _left, float _up, Color _color, int _life)
{
	colliderType = COL2D_BOX;
	pos_x = x_pos;
	pos_y = y_pos;
	dir_x = x_dir;
	dir_y = y_dir;
	speed = _speed;
	left = _left;
	up = _up;
	color = _color;
	life = _life;
}

void Box::draw()
{
	glPushMatrix();
	glTranslatef(pos_x, pos_y, 0);
	glBegin(GL_QUADS);//顺序为逆时针
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(-left, up, 0);
	glVertex3f(-left, -up, 0);
	glVertex3f(left, -up, 0);
	glVertex3f(left, up, 0);
	glEnd();
	glPopMatrix();
}

bool Box::testCollision(Collider2D* collider)
{
	Wall* wall_other = (Wall*)collider;
	bool result = false;
	switch (collider->colliderType){
	case COL2D_WALL:
		if (pos_x > wall_other->pos_x && (pos_x+dir_x*speed) - wall_other->pos_x <= left + wall_other->left + CBOFFSET&& abs(pos_y-wall_other->pos_y)<=up+wall_other->up && dir_x < 0){//撞墙的右面
			pos_x = wall_other->pos_x + wall_other->left + left + CBOFFSET;
		}
		else if (wall_other->pos_x > pos_x  && wall_other->pos_x - (pos_x + dir_x*speed) <= left + wall_other->left + CBOFFSET&& abs(pos_y - wall_other->pos_y) <= up + wall_other->up && dir_x > 0){//撞墙的左面
			pos_x = wall_other->pos_x - wall_other->left - left - CBOFFSET;
		}
		else if (pos_y > wall_other->pos_y && (pos_y + dir_y*speed) - wall_other->pos_y <= up + wall_other->up + CBOFFSET&& abs(pos_x - wall_other->pos_x) <= left + wall_other->left && dir_y < 0){//撞墙的上面
			pos_y = wall_other->pos_y + wall_other->up + up + CBOFFSET;
		}
		else if (wall_other->pos_y > pos_y  && wall_other->pos_y - (pos_y + dir_y*speed) <= up + wall_other->up + CBOFFSET&& abs(pos_x - wall_other->pos_x) <= left + wall_other->left && dir_y > 0){//撞墙的下面
			pos_y = wall_other->pos_y - wall_other->up - up - CBOFFSET;
		}
		result = true;
		break;
	default:
		break;
	}
	return result;
}