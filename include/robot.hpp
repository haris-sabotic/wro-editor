#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "glm/glm.hpp"
#include "record.hpp"
#include "util.hpp"
#include <cstddef>

struct RobotData {
    Rect rect;
    float rotation;
    unsigned int texture_id;
};

Rect adjust_robot_rect_to_screen(Rect robot_rect, float map_screen_width,
                                 float map_screen_height);

// move/rotate robot according to the instruction passed
void transform_robot_per_instruction(RobotData &robot_data,
                                     const Instruction *instruction);

// run all previous instructions so that the current one starts with the right
// transform
void transform_robot_until_instruction(RobotData &robot_data,
                                       const Program &program,
                                       size_t instruction_index);

#endif // ROBOT_HPP
