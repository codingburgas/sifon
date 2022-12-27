#pragma once
#include <raylib.h>
#include <vector>

using Vec2f = Vector2;
using Vec3f = Vector3;
using Vec4f = Vector4;
using Rectf = Rectangle;
using Polygon = std::vector<Vec2f>;

struct Vec2i {
    int x;
    int y;

    Vec2i (int _x, int _y) : x(_x), y(_y) {};
    Vec2i () : x(0), y(0) {};

    Vec2i (const Vector2& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); }
    operator Vector2() { return Vector2 { static_cast<float>(x), static_cast<float>(y) }; }
};

struct Vec3i {
    int x;
    int y;
    int z;

    Vec3i (int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
    Vec3i () : x(0), y(0), z(0) {};

    Vec3i (const Vector3& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); z = static_cast<int>(v.z); }
    operator Vector3() { return Vector3 { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }; }
};

struct Vec4i {
    int x;
    int y;
    int z;
    int w;

    Vec4i (int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {};
    Vec4i () : x(0), y(0), z(0), w(0) {};

    Vec4i (const Vector4& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); z = static_cast<int>(v.z); w = static_cast<int>(v.w); }
    operator Vector4() { return Vector4 { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w) }; }
};

struct Recti {
    int x;
    int y;
    int width;
    int height;

    Recti (int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h) {};
    Recti () : x(0), y(0), width(0), height(0) {};

    Recti (const Rectangle& r) { x = static_cast<int>(r.x); y = static_cast<int>(r.y); width = static_cast<int>(r.width); height = static_cast<int>(r.height); }
    operator Rectangle() { return Rectangle { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }; }
};

// float lerpf(float a, float b, float t)
// {
//     return a + (b - a) * t;
// }