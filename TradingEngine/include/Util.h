#pragma once
#include <math.h>

static const float TICK = 0.01f;

/*Check if two float numbers are the same within tick precision*/
static bool FloatEqual(float a, float b)
{
    return abs(a - b) < TICK;
}
