#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "glm/glm.hpp"
#include "util.hpp"

struct RobotData {
    Rect rect;
    float rotation;

    Rect screen_rect(const Rect &map_rect, int board_width,
                                 int board_height) const;

    RobotData(Rect _rect, float _rotation) : rect(_rect), rotation(_rotation){};
};

#endif // ROBOT_HPP
