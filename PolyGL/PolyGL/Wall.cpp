#include "Wall.h"


Wall::Wall()
{
	colliderType = COL2D_WALL;
	left = 1;
	up = 1;
	color = Color(0, 0.5, 0.5);
}

Wall::Wall(float x_pos, float y_pos, float _left, float _up, Color _color)
{
	colliderType = COL2D_WALL;
	left = _left;
	up = _up;
	pos_x = x_pos;
	pos_y = y_pos;
	color = _color;
}


Wall::~Wall()
{
}

bool Wall::testCollision(Collider2D* collider)
{
	return false;
}

void Wall::draw()
{
	glPushMatrix();
	glTranslatef(pos_x, pos_y, 0);
	glBegin(GL_QUADS);//Ë³ÐòÎªÄæÊ±Õë
	glColor3f(color.red, color.green, color.blue);
	glVertex3f(-left, up, 0);
	glVertex3f(-left, -up, 0);
	glVertex3f(left, -up, 0);
	glVertex3f(left, up, 0);
	glEnd();
	glPopMatrix();
}