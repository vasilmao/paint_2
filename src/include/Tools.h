#ifndef INCLUDE_TOOLS
#define INCLUDE_TOOLS

// #define _GNU_SOURCE

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <dlfcn.h>

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
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
    Color() {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }
    constexpr Color(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1) {
        r = r1;
        g = g1;
        b = b1;
        a = a1;
    }
    Color(uint32_t color_int) { // no explicit
        // r = (color_int & 0xFF000000) >> 24;
        // g = (color_int & 0xFF0000) >> 16;
        // b = (color_int & 0xFF00) >> 8;
        // a = (color_int &     0xFF);
        a = (color_int & 0xFF000000) >> 24;
        b = (color_int & 0xFF0000) >> 16;
        g = (color_int & 0xFF00) >> 8;
        r = (color_int & 0xFF);
    }
};

struct Colorf {
    float r;
    float g;
    float b;
};

#define open_color(color) color.r, color.g, color.b, color.a
#define open_colorf(color) color.r, color.g, color.b
#define int32_color(color) (((uint32_t)color.r) << 24) + (((uint32_t)color.g) << 16) + (((uint32_t)color.b) << 8) + (uint32_t)color.a


template<class T>
void Swap(T* el1, T* el2) {
    T tmp = *el1;
    *el1 = *el2;
    *el2 = tmp;
}

float power(float x, int p);

#endif