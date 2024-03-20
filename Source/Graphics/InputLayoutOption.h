#pragma once

enum InputLayoutOption
{
    Position = 1,
    VertexColor = 2,
    Normal = 4,
    Tangent = 8,
    UV0 = 16,
};

inline InputLayoutOption operator |(const InputLayoutOption a, const InputLayoutOption b)
{
    return static_cast<InputLayoutOption>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool HasFlag(const InputLayoutOption value, const InputLayoutOption flag)
{
    return (static_cast<int>(value) & static_cast<int>(flag)) != 0;
}