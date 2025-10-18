#pragma once

class Vec3D {
public:
    Vec3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    float x, y, z;
    float getLength();
};