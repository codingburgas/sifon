#include "la.hpp"

#pragma region Vector2_Operators
Vec2f operator+(Vec2f left, Vec2f right)
{
    return Vec2f{ left.x + right.x, left.y + right.y };
}
Vec2f operator-(Vec2f left, Vec2f right)
{
    return Vec2f{ left.x - right.x, left.y - right.y };
}
Vec2f operator*(Vec2f left, Vec2f right)
{
    return Vec2f{ left.x * right.x, left.y * right.y };
}
Vec2f operator/(Vec2f left, Vec2f right)
{
    return Vec2f{ left.x / right.x, left.y / right.y };
}

Vec2f operator+(Vec2f left, float right)
{
    return Vec2f{ left.x + right, left.y + right };
}
Vec2f operator-(Vec2f left, float right)
{
    return Vec2f{ left.x - right, left.y - right };
}
Vec2f operator*(Vec2f left, float right)
{
    return Vec2f{ left.x * right, left.y * right };
}
Vec2f operator/(Vec2f left, float right)
{
    return Vec2f{ left.x / right, left.y / right };
}
#pragma endregion