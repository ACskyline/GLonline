#include "Polygon.h"


Polygon::Polygon()
{
	colliderType = COL2D_POLYGON;
	polygon_vertexNumber = 3;
	polygon_vertexRange = 1;
	polygon_vertexArrayX = new float[polygon_vertexNumber];
	polygon_vertexArrayY = new float[polygon_vertexNumber];
	color = Color(0, 0, 1);
	generateVertices();
}


Polygon::~Polygon()
{
}

Polygon::Polygon(float x_pos, float y_pos, float _speed, float x_dir, float y_dir, int vertexNumber_polygon, int vertexRange_polygon, Color _color)
{
	colliderType = COL2D_POLYGON;
	pos_x = x_pos;
	pos_y = y_pos;
	dir_x = x_dir;
	dir_y = y_dir;
	speed = _speed;
	polygon_vertexNumber = vertexNumber_polygon;
	polygon_vertexRange = vertexRange_polygon;
	polygon_vertexArrayX = new float[polygon_vertexNumber];
	polygon_vertexArrayY = new float[polygon_vertexNumber];
	color = _color;
	generateVertices();
}

void Polygon::generateVertices()
{
	for (int i = 0; i < polygon_vertexNumber; i++){
		polygon_vertexArrayX[i] = randomNumber(polygon_vertexRange);
		polygon_vertexArrayY[i] = randomNumber(polygon_vertexRange);
	}
	//排序并生成凸多边形
	//
	//
	////////////////////
}

float Polygon::randomNumber(int max)
{
	float result = 0;
	result = rand() % 101 / 100 * max;
	if (rand() % 2 == 1){
		result = -result;
	}
	return result;
}

void Polygon::draw()
{
}

bool Polygon::testCollision(Collider2D* collider)
{
	return false;
}