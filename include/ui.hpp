#ifndef UI_HPP
#define UI_HPP

#include <unordered_map>
#include <GLFW/glfw3.h>
#include "robot.hpp"
#include "record.hpp"

// Everything related to imgui
namespace ui {
// ImGui stuff
void init(GLFWwindow *window);
void destroy();
void new_frame();
void render_frame();
void render_gl_draw_data();

// Window showing the player's position and rotation
void robot_transform(RobotData &robot_data, bool disabled);

// List of programs and their instructions
//
// Lets you add/remove programs and instructions
void programs(std::vector<Program> &programs, Instruction **currently_recording,
              RobotData &robot_data);

// Window that shows up when you start recording an instruction
//
// Lets you choose a type, set the motor speed and count(degrees rotated or
// distance passed depending on the instruction type)
//
// Also the motor_speeds map argument remembers motor speeds for each
// instruction type so that when you create a new instruction you get the last
// value instead of just 0
void record(Instruction **currently_recording, RobotData &robot_data,
            std::unordered_map<InstructionType, float> &motor_speeds);
} // namespace ui

#endif // UI_HPP
