#include "Vector.h"

Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z) {}

Vector3::Vector3(const Vector3&other): x(other.x), y(other.y), z(other.z) {}

float Vector3::getX() const {
    return x;
}

float Vector3::getY() const {
    return y;
}

float Vector3::getZ() const {
    return z;
}

void Vector3::setX(float new_x) {
    x = new_x;
}

void Vector3::setY(float new_y) {
    y = new_y;
}

void Vector3::setZ(float new_z) {
    z = new_z;
}


float Vector3::getLength() const {
    return sqrt(x * x + y * y + z * z);
}

void Vector3::normalize() {
    float length = getLength();
    x /= length;
    y /= length;
    z /= length;
}

Vector3& operator += (Vector3& v1, const Vector3& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

Vector3& operator -= (Vector3& v1, const Vector3& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

Vector3 operator + (const Vector3& v1, const Vector3& v2) {
    Vector3 result(v1);
    result += v2;
    return result;
}

Vector3 operator - (const Vector3& v1, const Vector3& v2) {
    Vector3 result(v1);
    result -= v2;
    return result;
}

Vector3& operator *= (Vector3& vector, const float coef) {
    vector.x *= coef;
    vector.y *= coef;
    vector.z *= coef;
    return vector;
}

Vector3 operator * (const Vector3& vector, const float coef) {
    Vector3 result(vector);
    result *= coef;
    return result;
}

Vector3 operator * (const float coef, const Vector3& vector) {
    Vector3 result(vector);
    result *= coef;
    return result;
}

float operator * (const Vector3& v1, const Vector3& v2) {
    float result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return result;
}

Vector3 Vector3::elByElMult(const Vector3& v2) const {
    return Vector3(x * v2.x, y * v2.y, z * v2.z);
}

Vector3 Vector3::getProjection(const Vector3& other) const {
    return (*this) * other / other.getLength() * other * (1 / other.getLength());
}

//-------------------------------------------------------------------------------

Vector2::Vector2(float x, float y): x(x), y(y) {}

Vector2::Vector2(const Vector2& other): x(other.x), y(other.y) {}

float Vector2::getX() const {
    return x;
}

float Vector2::getY() const {
    return y;
}

void Vector2::setX(float new_x) {
    x = new_x;
}

void Vector2::setY(float new_y) {
    y = new_y;
}

float Vector2::getLength() const {
    return sqrt(x * x + y * y);
}

void Vector2::normalize() {
    float length = getLength();
    x /= length;
    y /= length;
}

Vector2& operator += (Vector2& v1, const Vector2& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

Vector2& operator -= (Vector2& v1, const Vector2& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

Vector2 operator + (const Vector2& v1, const Vector2& v2) {
    Vector2 result(v1);
    result += v2;
    return result;
}

Vector2 operator - (const Vector2& v1, const Vector2& v2) {
    Vector2 result(v1);
    result -= v2;
    return result;
}

Vector2& operator *= (Vector2& vector, const float coef) {
    vector.x *= coef;
    vector.y *= coef;
    return vector;
}

Vector2 operator * (const Vector2& vector, const float coef) {
    Vector2 result(vector);
    result *= coef;
    return result;
}

Vector2 operator * (const float coef, const Vector2& vector) {
    Vector2 result(vector);
    result *= coef;
    return result;
}

float operator * (const Vector2& v1, const Vector2& v2) {
    float result = v1.x * v2.x + v1.y * v2.y;
    return result;
}

Vector2 Vector2::elByElMult(const Vector2& v2) const {
    return Vector2(x * v2.x, y * v2.y);
}

Vector2 Vector2::getProjection(const Vector2& other) const {
    return (*this) * other / other.getLength() * other * (1 / other.getLength());
}

float angle_cos(const Vector3& v1, const Vector3& v2) {
    return (v1 * v2) / v1.getLength() / v2.getLength();
}

float angle_cos(const Vector2& v1, const Vector2& v2) {
    return (v1 * v2) / v1.getLength() / v2.getLength();
}