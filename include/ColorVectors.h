#pragma once

#ifndef COLOR_VECTORS
#define COLOR_VECTORS
#include <algorithm>
#endif

class rgbVec
{
public:
    rgbVec(float red, float green, float blue)
    {
        // Min & Max Checks
        red = std::clamp(red, 0.f, 255.f);
        green = std::clamp(green, 0.f, 255.f);
        blue = std::clamp(blue, 0.f, 255.f);

        // Sets
        r = red;
        g = green;
        b = blue;
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

class rgbaVec
{
public:
    rgbaVec(float red, float green, float blue, float alpha)
    {
        // Min & Max Checks
        red = std::clamp(red, 0.f, 255.f);
        green = std::clamp(green, 0.f, 255.f);
        blue = std::clamp(blue, 0.f, 255.f);
        alpha = std::clamp(alpha, 0.f, 255.f);

        // Sets
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};