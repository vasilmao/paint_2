// #include "RayCaster.h"
// #include "Objects.h"

// const float ambient_light_intensity = 0.15;
// const float diffusion_coefficient   = 1;
// const float specular_coefficient    = 1;
// const int   specular_power          = 10;

// float getDiffusionIntensity(const Sphere* spere, const Light* light, const Vector3& point);
// float getSpecularIntensity (const Sphere* spere, const Light* light, const Camera* camera, const Vector3& point);

// void renderSphere(Renderer* renderer, const Sphere* sphere, const Camera* camera, const Light* light) {
//     const Vector2& camera_view = camera->getViewArea();
//     const Vector3& light_color  = light->getColorV();
//     const Vector3& sphere_color = sphere->getColorV();
//     float camera_range_y = camera_view.getY() / 2;
//     float camera_range_x = camera_view.getX() / 2;
//     for (float scan_screen_y = -camera_range_y; scan_screen_y <= camera_range_y; scan_screen_y += 1) {
//         for (float scan_screen_x = -camera_range_x; scan_screen_x <= camera_range_x; scan_screen_x += 1) {
//             // need to get z;
//             Vector3 result_color = {0, 0, 0};
//             if (sphere->intersecsRay(scan_screen_x, scan_screen_y)) {
//                 // renderer->drawPoint({scan_screen_x + camera_range_x, scan_screen_y + camera_range_y}, {255, 255, 255, 255});
//                 float z = sphere->computeZByXY(scan_screen_x, scan_screen_y);
//                 Vector3 sphere_point(scan_screen_x, scan_screen_y, z);
//                 Vector3 light_mul_sphere_color = light_color.elByElMult(sphere_color);
//                 float diffusion_intensity = getDiffusionIntensity(sphere, light, sphere_point);
//                 float specular_intensity  = 0;
//                 specular_intensity = getSpecularIntensity(sphere, light, camera, sphere_point);


//                 result_color = ambient_light_intensity * light_mul_sphere_color;
//                 result_color += diffusion_intensity * light_mul_sphere_color;
//                 result_color += specular_intensity * light_color;

//                 result_color.setX(Min(1.0f, Max(0.0f, result_color.getX())));
//                 result_color.setY(Min(1.0f, Max(0.0f, result_color.getY())));
//                 result_color.setZ(Min(1.0f, Max(0.0f, result_color.getZ())));
//             }
//             Color render_color = {(unsigned char)(result_color.getX() * 255), (unsigned char)(result_color.getY() * 255), (unsigned char)(result_color.getZ() * 255), 255};
//             renderer->drawPoint(Vector2{scan_screen_x + camera_range_x, scan_screen_y + camera_range_y}, render_color);

//         }
//     }
// }

// float getDiffusionIntensity(const Sphere* sphere, const Light* light, const Vector3& point) {
//     Vector3 sphere_normal = point - sphere->getPos();
//     Vector3 light_direction = light->getPos() - point;
//     float alpha_cos = Max(0.0f, angle_cos(light_direction, sphere_normal));
//     return Min(1.0f, Max(0.0f, diffusion_coefficient * alpha_cos));
// }

// float getSpecularIntensity(const Sphere* sphere, const Light* light, const Camera* camera, const Vector3& point) {
//     Vector3 sphere_normal = point - sphere->getPos();
//     Vector3 direction_to_light = light->getPos() - point;
//     Vector3 light_dir_perp = direction_to_light - direction_to_light.getProjection(sphere_normal);
//     Vector3 direction_to_light_reflection = direction_to_light - 2 * light_dir_perp;
//     Vector3 direction_to_camera(0, 0, camera->getPos().getZ() - point.getZ());
//     float betta_cos = Max(0.0f, angle_cos(direction_to_light_reflection, direction_to_camera));
//     return Min(1.0f, Max(0.0f, power(betta_cos, specular_power) * specular_coefficient));
// }