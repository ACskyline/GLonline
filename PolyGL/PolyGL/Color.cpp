#include "Color.h"


Color::Color()
{
	red = 1;
	green = 1;
	blue = 1;
}

Color::Color(float r,float g,float b)
{
	red = r;
	green = g;
	blue = b;
}

Color::~Color()
{
}

bool Color::equalTo(Color other)
{
	if (red == other.red&&green == other.green&&blue == other.blue){
		return true;
	}
	else
		return false;
}