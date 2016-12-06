#pragma once
#include "Collider2D.h"
#include "Wall.h"
#include "Box.h"
class Ball : public Collider2D
{
public:
	float ball_radius;
	int ball_slice;
	Ball();
	Ball(float x_pos, float y_pos, float _speed, float x_dir = 0, float y_dir = 0, float radius_ball = 1, int slice_ball = 50, Color _color = Color(1, 0, 0));
	~Ball();
	void draw();
	bool testCollision(Collider2D* collider2d_other);
	bool collideBoxVertex(Box* box_other);
};

