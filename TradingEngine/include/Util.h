#pragma once
#include <math.h>

static const float TICK = 0.01f;

/*Check if two float numbers are the same within tick precision*/
static bool FloatEqual(float a, float b)
{
    return abs(a - b) < TICK;
}

static float getRandomNumber()
{
	float LO = 0.7f;
	float HI = 1.5f;
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

static float round2(float n)
{
	return round(n * 100) / 100;
}

static float Randomize(float n)
{
	return round2(n * getRandomNumber());
}