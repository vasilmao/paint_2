#ifndef INCLUDE_COORD_SYSTEM
#define INCLUDE_COORD_SYSTEM
#include "Tools.h"
#include "Vector.h"

// class PixelCoordSystem {

// private:
//     float x_min = 0;
//     float y_min = 0;
//     float x_max = 0;
//     float y_max = 0;

//     float pixel_x = 0;
//     float pixel_y = 0;
//     float pixel_width  = 0;
//     float pixel_height = 0;

//     float point_to_pixel_coeff_x = 0;
//     float point_to_pixel_coeff_y = 0;

// public:
//     PixelCoordSystem(){}
//     ~PixelCoordSystem(){
//         printf("coord system destroyed!\n");
//     }

//     PixelCoordSystem(Rect2f range_rect, Rect2f pixel_rect);

//     Rect2f getPixelRect() const;
//     Rect2f getCoordRect() const;
//     Vector translatePoint(const Vector& point) const;
//     Vector translatePixel(const Vector& pixel) const;
// };

class CoordSystem {

private:
    Vector2 relative_pos;
    Vector2 relative_size;

    Vector2 absolute_pos;
    Vector2 absolute_size;

    float rel_to_abs_coeff_x = 0;
    float rel_to_abs_coeff_y = 0;

public:
    CoordSystem(){}
    ~CoordSystem(){
        printf("coord system destroyed!\n");
    }

    CoordSystem(Rect2f range_rect, Rect2f pixel_rect);

    // Rect2f getPixelRect() const;
    // Rect2f getCoordRect() const;
    Vector2 translateToRelative(const Vector2& point) const;
    Vector2 translateToAbsolute(const Vector2& point) const;
    void setRelativeSize(const Vector2& new_size);
    void setAbsoluteSize(const Vector2& new_size);
    void setRelativePos(const Vector2& new_pos);
    void setAbsolutePos(const Vector2& new_pos);
    const Vector2& getRelativeSize();
    const Vector2& getAbsoluteSize();
    const Vector2& getRelativePos();
    const Vector2& getAbsolutePos();
};

#endif