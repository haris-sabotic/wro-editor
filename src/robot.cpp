#include "robot.hpp"

Rect RobotData::screen_rect(const Rect &map_rect, int board_width,
                            int board_height) const {
    float ux = rect.x / board_width;
    float uy = rect.y / board_height;

    float uw = rect.width / board_width;
    float uh = rect.height / board_height;

    return Rect(map_rect.width * ux, map_rect.height * uy, map_rect.width * uw,
                map_rect.height * uh);
}

void transform_robot_per_instruction(RobotData &robot_data,
                                     const Instruction *instruction) {
    if (instruction->type == InstructionType::SPIN_TURN) {
        robot_data.rotation += instruction->count;
    } else if (instruction->type == InstructionType::MOVE_STRAIGHT) {
        robot_data.rect.x +=
            instruction->count *
            glm::cos(glm::radians(robot_data.rotation - 90.0f));
        robot_data.rect.y +=
            instruction->count *
            glm::sin(glm::radians(robot_data.rotation - 90.0f));
    } else if (instruction->type == InstructionType::PIVOT_TURN_RIGHT) {
        robot_data.rotation += instruction->count;
        robot_data.rect.x +=
            instruction->count * glm::cos(glm::radians(robot_data.rotation));
        robot_data.rect.y +=
            instruction->count * glm::sin(glm::radians(robot_data.rotation));
    } else if (instruction->type == InstructionType::PIVOT_TURN_LEFT) {
        robot_data.rotation += instruction->count;
        robot_data.rect.y +=
            instruction->count * glm::cos(glm::radians(robot_data.rotation));
        robot_data.rect.x +=
            instruction->count * glm::sin(glm::radians(robot_data.rotation));
    }
}

void transform_robot_until_instruction(RobotData &robot_data,
                                       const Program &program,
                                       size_t instruction_index) {
    robot_data.rect.x = program.start_x;
    robot_data.rect.y = program.start_y;
    robot_data.rotation = program.start_rotation;
    for (size_t i = 0; i < instruction_index; i++) {
        transform_robot_per_instruction(robot_data, &program.instructions[i]);
    }
}
