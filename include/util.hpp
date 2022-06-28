#ifndef UTIL_HPP
#define UTIL_HPP

#include <string_view>

struct Rect {
    float x, y;
    float width, height;

    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(float x, float y, float width, float height);
};

struct Texture {
    // OpenGL id you get from glGenTextures
    unsigned int id;

    /// Original dimensions of the picture
    unsigned int width;
    unsigned int height;

    Texture() : id(0), width(0), height(0) {}
};

Texture load_texture_from_file(std::string_view path);

// Used for fitting the map entirely on screen - automatically fitting it by
// width or height
Rect auto_fit_rect_in_rect(Rect out_rect, Rect in_rect);

#endif // UTIL_HPP
