#ifndef INCLUDE_TOOLS
#define INCLUDE_TOOLS

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

const float EPS = 1e-6;

template<class T>
const T& Max(const T& el1, const T& el2) {
    return el1 < el2 ? el2 : el1;
}

template<class T>
const T& Min(const T& el1, const T& el2) {
    return el1 < el2 ? el1 : el2;
}

struct Rect2i {
    int x;
    int y;
    int width;
    int height;
};

struct Rect2f {
    float x;
    float y;
    float width;
    float height;
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct Colorf {
    float r;
    float g;
    float b;
    float a;
};

#define open_color(color) color.r, color.g, color.b, color.a
#define open_color01(color) color.r, color.g, color.b
#define int32_color(color) (((uint32_t)color.r) << 24) + (((uint32_t)color.g) << 16) + (((uint32_t)color.b) << 8) + (uint32_t)color.a


template<class T>
void Swap(T* el1, T* el2) {
    T tmp = *el1;
    *el1 = *el2;
    *el2 = tmp;
}

float power(float x, int p);

#endif