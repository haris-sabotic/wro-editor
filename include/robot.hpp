#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "glm/glm.hpp"
#include "record.hpp"
#include "util.hpp"
#include <cstddef>

struct RobotData {
    Rect rect;
    float rotation;

    RobotData() : rect(Rect()), rotation(0.0f) {}

    Rect screen_rect(const Rect &map_rect, int board_width,
                     int board_height) const;

    RobotData(Rect _rect, float _rotation) : rect(_rect), rotation(_rotation){};
};

// move/rotate robot according to the instruction passed
void transform_robot_per_instruction(RobotData &robot_data,
                                     const Instruction *instruction);

// run all previous instructions so that the current one starts with the right
// transform
void transform_robot_until_instruction(RobotData &robot_data,
                                       const Program &program,
                                       size_t instruction_index);

#endif // ROBOT_HPP
