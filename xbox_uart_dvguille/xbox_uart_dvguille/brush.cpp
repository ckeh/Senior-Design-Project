#include "brush.h"



Brush::Brush(D2D1_COLOR_F &color):
	color(color)
{
}


Brush::Brush(D2D1_COLOR_F & color, int gradientNumber)
{
	gradientStopsArray = new D2D1_GRADIENT_STOP[gradientNumber];
}

Brush::~Brush()
{
	delete [] gradientStopsArray;
}
