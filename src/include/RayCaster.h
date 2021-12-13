#ifndef INCLUDE_RAY_CASTER
#define INCLUDE_RAY_CASTER

#include "GraphicLib.h"
// #include "Objects.h"

class Light;
class Camera;
class Sphere;


void renderSphere(const Renderer* renderer, const Sphere* sphere, const Camera* camera, const Light* light);

#endif