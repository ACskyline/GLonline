#pragma once
#include "Collider2D.h"
class Box : public Collider2D
{
public:
	float left;//right = left
	float up;//down = up
	int life;
	Box();
	Box(float x_pos, float y_pos, float _speed, float x_dir = 0, float y_dir = 0, float _left = 0.1, float _up = 0.1, Color _color = Color(0.1, 0.2, 0.3),int _life = 1);
	~Box();
	void draw();
	bool testCollision(Collider2D* collider);
};

