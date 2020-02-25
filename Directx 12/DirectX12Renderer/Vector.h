#pragma once

#include "Renderer.h"

struct VFLOAT3
{
    float x;
    float y;
    float z;

    VFLOAT3() = default;

    VFLOAT3(const VFLOAT3&) = default;
    VFLOAT3& operator =(const VFLOAT3&) = default;

    VFLOAT3(VFLOAT3&&) = default;
    VFLOAT3& operator=(VFLOAT3&&) = default;

    constexpr VFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    explicit VFLOAT3(_In_reads_(3) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
};

struct VFLOAT4
{
    float x;
    float y;
    float z;
    float w;

    VFLOAT4() = default;

    VFLOAT4(const VFLOAT4&) = default;
    VFLOAT4& operator=(const VFLOAT4&) = default;

    VFLOAT4(VFLOAT4&&) = default;
    VFLOAT4& operator=(VFLOAT4&&) = default;

    constexpr VFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    explicit VFLOAT4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
};