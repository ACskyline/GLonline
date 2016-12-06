#pragma once
#include "Collider2D.h"
#include <iostream>
//只有横着的和竖着的，不会检测倾斜情况下的碰撞
class Wall : public Collider2D
{
public:
	float left;//right = left
	float up;//down = up
	Wall();
	Wall(float x_pos, float y_pos, float _left = 1, float _up = 1, Color _color = Color(0, 0.5, 0.5));
	~Wall();
	void draw();
	bool testCollision(Collider2D* collider);
};

