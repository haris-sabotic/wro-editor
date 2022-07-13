#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <vector>

// TODO: Maybe add instructions for the arm/claw and going up to a black line
enum class InstructionType {
    NOOP,
    MOVE_STRAIGHT,
    SPIN_TURN,        // rotate around the center with both motors
    PIVOT_TURN_LEFT,  // keep left motor still and rotate around it with the
                      // right motor
    PIVOT_TURN_RIGHT, // keep right motor still and rotate around it with the
                      // left motor
};

struct Instruction {
    InstructionType type;
    float count; // degrees rotated(in degrees) or distance passed(in motor
                 // rotations) depending on the type of instruction
    float speed; // motor speed(in percentages)

    Instruction() : type(InstructionType::NOOP), count(0.0f), speed(0.0f) {}

    Instruction(InstructionType type, float count, float speed)
        : type(type), count(count), speed(speed) {}
};

struct Program {
    std::string name;
    std::vector<Instruction> instructions;

    // starting robot transform for the program
    float start_x, start_y;
    float start_rotation;

    Program(std::string name, std::vector<Instruction> instructions,
            float start_x, float start_y, float start_rotation)
        : name(name), instructions(instructions), start_x(start_x),
          start_y(start_y), start_rotation(start_rotation) {}
};

struct InstructionPlayData {
    Program *program = nullptr;
    size_t index = 0;
    float count_left;
    bool single_instruction;
};

#endif // RECORD_HPP
