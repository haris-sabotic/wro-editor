#include "robot.hpp"
#include "constants.hpp"

Rect adjust_robot_rect_to_screen(Rect robot_rect, float map_screen_width,
                                 float map_screen_height) {
    float ux = robot_rect.x / MAP_REAL_WIDTH;
    float uy = robot_rect.y / MAP_REAL_HEIGHT;

    float uw = robot_rect.width / MAP_REAL_WIDTH;
    float uh = robot_rect.height / MAP_REAL_HEIGHT;

    return Rect{map_screen_width * ux, map_screen_height * uy,
                map_screen_width * uw, map_screen_height * uh};
}

void transform_robot_per_instruction(RobotData &robot_data,
                                     const Instruction *instruction) {
    if (instruction->type == InstructionType::SPIN_TURN) {
        robot_data.rotation += instruction->count;
    } else if (instruction->type == InstructionType::MOVE_STRAIGHT) {
        // the robot should move forward according to its current rotation, so I
        // can't just increase its y or x by the distance
        robot_data.rect.x +=
            DISTANCE_PASSED_PER_MOTOR_ROTATION * instruction->count *
            glm::cos(glm::radians(robot_data.rotation - 90.0f));
        robot_data.rect.y +=
            DISTANCE_PASSED_PER_MOTOR_ROTATION * instruction->count *
            glm::sin(glm::radians(robot_data.rotation - 90.0f));
    } else if (instruction->type == InstructionType::PIVOT_TURN_RIGHT) {
        float offsetx = glm::cos(glm::radians(robot_data.rotation)) *
                        (robot_data.rect.width / 2);
        float offsety = glm::sin(glm::radians(robot_data.rotation)) *
                        (robot_data.rect.width / 2);
        robot_data.rotation -= instruction->count;
        float s = glm::sin(glm::radians(robot_data.rotation));
        float c = glm::cos(glm::radians(robot_data.rotation));
        robot_data.rect.x -= c * (robot_data.rect.width / 2) - offsetx;
        robot_data.rect.y -= s * (robot_data.rect.width / 2) - offsety;
    } else if (instruction->type == InstructionType::PIVOT_TURN_LEFT) {
        float offsetx = glm::cos(glm::radians(robot_data.rotation)) *
                        (robot_data.rect.width / 2);
        float offsety = glm::sin(glm::radians(robot_data.rotation)) *
                        (robot_data.rect.width / 2);
        robot_data.rotation += instruction->count;
        float s = glm::sin(glm::radians(robot_data.rotation));
        float c = glm::cos(glm::radians(robot_data.rotation));
        robot_data.rect.x += c * (robot_data.rect.width / 2) - offsetx;
        robot_data.rect.y += s * (robot_data.rect.width / 2) - offsety;
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
