#pragma once

#pragma region INCLUDES/NAMESPACE
#include <algorithm>
#pragma endregion

#pragma region RGB
/// @brief R,G,B Vector
/// @author Caleb Brodock
/// @version 1
/// @date 9/25/2023
class rgbVec
{
public:
    /// @brief Class Structure for a RGB Vector.
    /// @param red RED value from 0->255. Defaults to 0.
    /// @param green GREEN value from 0->255. Defaults to 0.
    /// @param blue BLUE value from 0->255. Defaults to 0.
    /// @author Caleb Brodock
    /// @version 1
    /// @date 9/25/2023
    rgbVec(float red = 0.f, float green = 0.f, float blue = 0.f) : r(std::clamp(red, 0.f, 255.f)),
                                                                   g(std::clamp(green, 0.f, 255.f)),
                                                                   b(std::clamp(blue, 0.f, 255.f))
    {
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};
#pragma endregion

#pragma region RGBA
/// @brief R,G,B,A Vector
/// @author Caleb Brodock
/// @version 1
/// @date 9/25/2023
class rgbaVec
{
public:
    /// @brief Class Structure for a RGBA Vector.
    /// @param red RED value from 0->255. Defaults to 0.
    /// @param green GREEN value from 0->255. Defaults to 0.
    /// @param blue BLUE value from 0->255. Defaults to 0.
    /// @param alpha ALPHA value from 0->255. Defaults to 255.
    /// @author Caleb Brodock
    /// @version 1
    /// @date 9/25/2023
    rgbaVec(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 255.f) : r(std::clamp(red, 0.f, 255.f)),
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
#pragma endregion