#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <vector>

enum class InstructionType {
    NOOP,
    MOVE_STRAIGHT,
    SPIN_TURN,
    PIVOT_TURN_LEFT,
    PIVOT_TURN_RIGHT,
};

struct Instruction {
    InstructionType type;
    float rotations;
    float speed;

    Instruction(InstructionType type, float rotations, float speed)
        : type(type), rotations(rotations), speed(speed) {}
};

struct Program {
    std::string name;
    std::vector<Instruction> instructions;

    float start_x, start_y;
    float start_rotation;

    Program(std::string name, std::vector<Instruction> instructions,
            float start_x, float start_y, float start_rotation)
        : name(name), instructions(instructions), start_x(start_x),
          start_y(start_y), start_rotation(start_rotation) {}
};

#endif // RECORD_HPP
