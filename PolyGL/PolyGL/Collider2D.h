#pragma once
#include <GL\glut.h>
#include <stdlib.h>
#include <math.h>
#include "Color.h"
enum ColliderType{
	COL2D_UNDEFINED, COL2D_BALL, COL2D_POLYGON, COL2D_WALL, COL2D_BOX
};

class Collider2D
{
public:
	float speed;
	//(dir_x,dir_y)必须是单位向量
	float dir_x;
	//(dir_x,dir_y)必须是单位向量
	float dir_y;
	float pos_x;
	float pos_y;
	Color color;
	ColliderType colliderType;
	Collider2D();
	~Collider2D();
	virtual bool testCollision(Collider2D* collider) = 0;
	virtual void draw() = 0;
	void update();
	void normalize(float *a, float *b);
	float magnitude(float a, float b);
	float distance(float x1, float y1, float x2, float y2);
	bool away(Collider2D *collider);
	bool awayVertex(float x, float y);
};

