#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "glm/glm.hpp"

const int MAP_REAL_WIDTH = 2232;
const int MAP_REAL_HEIGHT = 1080;

const float GAME_START_X = 0.0f;
const float GAME_START_Y = -1.5f;
const float GAME_START_ROTATION = -45.0f;

const float DISTANCE_PASSED_PER_MOTOR_ROTATION = 120.0f;

const glm::vec4 ROBOT_DIRECTION_GUIDE_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);

// Distance passed over 1 second at 100% motor speed
const float MOVE_STRAIGHT_SPEED = 1.0f;
// Degrees rotated over 1 second at 100% motor speed
const float SPIN_TURN_SPEED = 180.0f;
// Degrees rotated over 1 second at 100% motor speed
const float PIVOT_TURN_SPEED = 90.0f;

#endif // CONSTANTS_HPP
