#pragma once
#include <math.h>

static const float TICK = 0.01f;

static bool FloatEqual(float a, float b)
{
    return abs(a - b) < TICK;
}