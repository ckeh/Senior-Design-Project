#pragma once
#include "includes.h"

enum BrushType {
	SOLID,
	LINEAR_GRADIENT,
	RADIAL_GRADIENT,
	BITMAP_BRUSH
};

class Brush
{
	friend class Graphics;
private:
	ID2D1Brush *brush;
	BrushType brushType;
	D2D1_COLOR_F color;
	int gradientNumber;
	D2D1_GRADIENT_STOP *gradientStopsArray;

public:
	// Solid Brush
	Brush(D2D1_COLOR_F &color);
	//LinearGradient Brush
	Brush(D2D1_COLOR_F &color, int gradientNumber);
	~Brush();

	// The color used for the Brush
	void Release() { brush->Release(); }
};

