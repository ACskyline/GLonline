#pragma once
#include "Collider2D.h"

class Polygon : public Collider2D
{
private:
	void generateVertices();
	float randomNumber(int max);
public:
	int polygon_vertexNumber;
	//顶点分布在pos_x-polygon_vertexRange到pos_x+polygon_vertexRange，pos_y-polygon_vertexRange到pos_y+polygon_vertexRange
	int polygon_vertexRange;
	float* polygon_vertexArrayX;
	float* polygon_vertexArrayY;
	Polygon();
	Polygon(float x_pos, float y_pos, float _speed, float x_dir = 0, float y_dir = 0, int vertexNumber_polygon = 3, int vertexRange_polygon = 1, Color _color = Color(0, 0, 1));
	~Polygon();
	void draw();
	bool testCollision(Collider2D* collider);
};

