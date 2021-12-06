#ifndef INCLUDE_VECTOR_CLASS
#define INCLUDE_VECTOR_CLASS

#include <stdio.h>
#include <math.h>
#include "Tools.h"

class Vector3 {

private:
    float x = 0;
    float y = 0;
    float z = 0;

public:
    Vector3(float x, float y, float z);
    Vector3(const Vector3& other);

    Vector3(){}
    ~Vector3(){}

    void update();
    float getX() const;
    float getY() const;
    float getZ() const;
    void setX(float new_x);
    void setY(float new_y);
    void setZ(float new_z);
    float getLength() const;
    void normalize();

    friend Vector3& operator += (Vector3& v1,     const Vector3& v2);
    friend Vector3& operator -= (Vector3& v1,     const Vector3& v2);
    friend Vector3 operator + (const Vector3& v1, const Vector3& v2);
    friend Vector3 operator - (const Vector3& v1, const Vector3& v2);
    friend Vector3& operator *= (Vector3& vector, const float coef);
    friend Vector3 operator * (const Vector3& vector, const float coef);
    friend Vector3 operator * (const float coef, const Vector3& vector);
    friend float operator * (const Vector3& v1, const Vector3& v2);
    Vector3 elByElMult(const Vector3& v2) const;
    Vector3 getProjection(const Vector3& other) const;

};

class Vector2 {

private:
    float x = 0;
    float y = 0;

public:
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    Vector2(){}
    ~Vector2(){}

    void update();
    float getX() const;
    float getY() const;
    void setX(float new_x);
    void setY(float new_y);
    float getLength() const;
    void normalize();

    friend Vector2& operator += (Vector2& v1,     const Vector2& v2);
    friend Vector2& operator -= (Vector2& v1,     const Vector2& v2);
    friend Vector2 operator + (const Vector2& v1, const Vector2& v2);
    friend Vector2 operator - (const Vector2& v1, const Vector2& v2);
    friend Vector2& operator *= (Vector2& vector, const float coef);
    friend Vector2 operator * (const Vector2& vector, const float coef);
    friend Vector2 operator * (const float coef, const Vector2& vector);
    friend float operator * (const Vector2& v1, const Vector2& v2);
    friend float operator ^ (const Vector2& v1, const Vector2& v2);
    Vector2 elByElMult(const Vector2& v2) const;
    Vector2 getProjection(const Vector2& other) const;
};

float angle_cos(const Vector3& v1, const Vector3& v2);
float angle_cos(const Vector2& v1, const Vector2& v2);

#endif