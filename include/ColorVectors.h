#pragma once

#ifndef COLOR_VECTORS_H
#define COLOR_VECTORS_H
#include <algorithm>
#endif // COLOR_VECTORS_H

class rgbVec
{
public:
    rgbVec(float red, float green, float blue) : r(std::clamp(red, 0.f, 255.f)),
                                                 g(std::clamp(green, 0.f, 255.f)),
                                                 b(std::clamp(blue, 0.f, 255.f))
    {
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

class rgbaVec
{
public:
    rgbaVec(float red, float green, float blue, float alpha) : r(std::clamp(red, 0.f, 255.f)),
                                                               g(std::clamp(green, 0.f, 255.f)),
                                                               b(std::clamp(blue, 0.f, 255.f)),
                                                               a(std::clamp(alpha, 0.f, 255.f))
    {
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};