#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "util.hpp"

struct RobotData {
    Rect rect;
    float rotation;

    RobotData(Rect _rect, float _rotation) : rect(_rect), rotation(_rotation){};
};

#endif // ROBOT_HPP
