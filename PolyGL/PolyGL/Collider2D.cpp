#include "Collider2D.h"


Collider2D::Collider2D()
{
	speed = 0;
	dir_x = 0;
	dir_y = 0;
	pos_x = 0;
	pos_y = 0;
	color = Color(0, 1, 0);
	colliderType = COL2D_UNDEFINED;
}


Collider2D::~Collider2D()
{
}

void Collider2D::normalize(float *a, float *b)
{
	float mag = magnitude(*a,*b);
	if (mag != 0){
		*a = *a / mag;
		*b = *b / mag;
	}
}

float Collider2D::magnitude(float a, float b)
{
	return sqrtf(a*a + b*b);
}

void Collider2D::update()
{
	pos_x += speed*dir_x;
	pos_y += speed*dir_y;
}

float Collider2D::distance(float x1, float y1, float x2, float y2)
{
	return sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

bool Collider2D::away(Collider2D* collider)
{
	normalize(&dir_x, &dir_y);
	normalize(&collider->dir_x, &collider->dir_y);
	if (distance(pos_x + dir_x*speed, pos_y + dir_y*speed, collider->pos_x + collider->dir_x*collider->speed, collider->pos_y + collider->dir_y*collider->speed) >= distance(pos_x, pos_y, collider->pos_x, collider->pos_y)){
		return true;
	}
	else{
		return false;
	}
}

bool Collider2D::awayVertex(float x, float y)
{
	normalize(&dir_x, &dir_y);
	if (distance(pos_x + dir_x*speed, pos_y + dir_y*speed, x, y) >= distance(pos_x, pos_y, x, y)){
		return true;
	}
	else{
		return false;
	}
}