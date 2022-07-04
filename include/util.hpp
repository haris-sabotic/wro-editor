#ifndef UTIL_HPP
#define UTIL_HPP

#include <string_view>
#include "record.hpp"

struct Rect {
    float x, y;
    float width, height;
};

struct Texture {
    // OpenGL id you get from glGenTextures
    unsigned int id;

    /// Original dimensions of the picture
    unsigned int width;
    unsigned int height;
};

Texture load_texture_from_file(std::string_view path);

// Used for fitting the map entirely on screen - automatically fitting it by
// width or height
Rect auto_fit_rect_in_rect(Rect out_rect, Rect in_rect);

constexpr const char *instruction_type_to_string(InstructionType t) {
    switch (t) {
    case InstructionType::NOOP:
        return "Empty";
    case InstructionType::MOVE_STRAIGHT:
        return "Move straight";
    case InstructionType::SPIN_TURN:
        return "Spin turn";
    case InstructionType::PIVOT_TURN_LEFT:
        return "Pivot turn left";
    case InstructionType::PIVOT_TURN_RIGHT:
        return "Pivot turn right";
    }
}

#endif // UTIL_HPP
