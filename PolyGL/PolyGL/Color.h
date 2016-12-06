#pragma once
#include <GL\glut.h>
class Color
{
public:
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	Color();
	Color(float r, float g, float b);
	~Color();
	bool equalTo(Color color_other);
};

