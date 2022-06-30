#ifndef UI_HPP
#define UI_HPP

#include <GLFW/glfw3.h>
#include "robot.hpp"
#include "record.hpp"

namespace ui {
void init(GLFWwindow *window);
void destroy();
void new_frame();
void render_frame();
void render_gl_draw_data();

void robot_transform(RobotData &robot_data);
void programs(std::vector<Program> &programs, Instruction **currently_recording,
              float robotx, float roboty, float robot_rotation);
void record(Instruction **currently_recording);
} // namespace ui

#endif // UI_HPP
