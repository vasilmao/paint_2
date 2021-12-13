#include "Objects.h"
#include "RayCaster.h"

Light::Light(const Vector3& pos, Colorf color) : pos(pos), color(color), color_v(open_colorf(color)) {}
const Vector3& Light::getPos() const {
    return pos;
}
void Light::setPos(const Vector3& new_pos) {
    pos = new_pos;
}
Colorf Light::getColor() const {
    return color;
}
const Vector3& Light::getColorV() const {
    return color_v;
}
void Light::setColor(Colorf new_color) {
    color = new_color;
    color_v = Vector3(open_colorf(color));
}
void Light::render(Renderer* renderer) {
    renderer->drawFilledCircle({pos.getX(), pos.getY()}, 5, {255, 255, 255, 255});
}


Camera::Camera(const Vector3& pos, const Vector3& direction, const Vector2& view_area) : pos(pos), direction(direction), view_area(view_area) {}
const Vector3& Camera::getPos() const {
    return pos;
}
const Vector3& Camera::getDirection() const {
    return direction;
}
const Vector2& Camera::getViewArea() const {
    return view_area;
}
void Camera::setPos(const Vector3& new_pos) {
    pos = new_pos;
}
void Camera::setDirection(const Vector3& new_direction) {
    direction = new_direction;
}
void Camera::setViewArea(const Vector2& new_view_area) {
    view_area = new_view_area;
}


Sphere::Sphere(const Vector3& pos, float r, Colorf color) : pos(pos), r(r), color(color), color_v(open_colorf(color)) {}
const Vector3& Sphere::getPos() const {
    return pos;
}
void Sphere::setPos(const Vector3& new_pos) {
    pos = new_pos;
}
float Sphere::getR() const {
    return r;
}
void Sphere::setR(float new_r) {
    r = new_r;
}
Colorf Sphere::getColor() const {
    return color;
}
const Vector3& Sphere::getColorV() const {
    return color_v;
}
void Sphere::setColor(Colorf new_color) {
    color = new_color;
    color_v = Vector3(open_colorf(color));
}
void Sphere::render(Renderer* renderer, Camera* camera, Light* light) {
    renderSphere(renderer, this, camera, light);
}
bool Sphere::intersecsRay(float x, float y) const {
    return (x * x + y * y <= r * r);
}
float Sphere::computeZByXY(float x, float y) const {
    return sqrt(r * r - x * x - y * y);
}
