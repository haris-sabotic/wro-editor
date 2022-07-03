#include "robot.hpp"

Rect adjust_robot_rect_to_screen(Rect robot_rect, float map_screen_width,
                                 float map_screen_height, float map_real_width,
                                 float map_real_height) {
    float ux = robot_rect.x / map_real_width;
    float uy = robot_rect.y / map_real_height;

    float uw = robot_rect.width / map_real_width;
    float uh = robot_rect.height / map_real_height;

    return Rect{map_screen_width * ux, map_screen_height * uy, map_screen_width * uw,
                map_screen_height * uh};
}

void transform_robot_per_instruction(RobotData &robot_data,
                                     const Instruction *instruction) {
    if (instruction->type == InstructionType::SPIN_TURN) {
        robot_data.rotation += instruction->count;
    } else if (instruction->type == InstructionType::MOVE_STRAIGHT) {
        // the robot should move forward according to its current rotation, so I can't just increase its y or x by the distance
        robot_data.rect.x +=
            instruction->count *
            glm::cos(glm::radians(robot_data.rotation - 90.0f));
        robot_data.rect.y +=
            instruction->count *
            glm::sin(glm::radians(robot_data.rotation - 90.0f));
    } else if (instruction->type == InstructionType::PIVOT_TURN_RIGHT) {
        // TODO: FIX THIS SHIT YOU MOTHERFUCKING RETARD
        robot_data.rotation += instruction->count;
        robot_data.rect.x +=
            instruction->count * glm::cos(glm::radians(robot_data.rotation));
        robot_data.rect.y +=
            instruction->count * glm::sin(glm::radians(robot_data.rotation));
    } else if (instruction->type == InstructionType::PIVOT_TURN_LEFT) {
        // TODO: FIX THIS SHIT YOU MOTHERFUCKING RETARD x2
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
    // go back to where the first instruction should've executed
    robot_data.rect.x = program.start_x;
    robot_data.rect.y = program.start_y;
    robot_data.rotation = program.start_rotation;
    // execute all the following instructions
    for (size_t i = 0; i < instruction_index; i++) {
        transform_robot_per_instruction(robot_data, &program.instructions[i]);
    }
}
