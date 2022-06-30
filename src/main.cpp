#include "game.hpp"
#include "ui.hpp"
#include "record.hpp"
#include <vector>

int main() {
    Game game;

    ui::init(game.window);

    RobotData robot(Rect(0.0f, 0.0f, 170.0f, 190.0f), -45.0f);

    std::vector<Program> programs = {
        Program("Program 1",
                {
                    Instruction(InstructionType::MOVE_STRAIGHT, 2.0, 50.0),
                    Instruction(InstructionType::SPIN_TURN, 90, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, -4.5, 50.0),
                    Instruction(InstructionType::PIVOT_TURN_LEFT, -90, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, 3.0, 70.0),
                },
                0.0f, 0.0f, -45.0f),
        Program("Program 2",
                {
                    Instruction(InstructionType::MOVE_STRAIGHT, -5.0, 50.0),
                    Instruction(InstructionType::SPIN_TURN, 45, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, 4.5, 70.0),
                },
                200.0f, -300.0f, 27.0f),
        Program("Program 3",
                {
                    Instruction(InstructionType::SPIN_TURN, 180, 40.0),
                },
                0.0f, 0.0f, 0.0f),
    };

    while (!glfwWindowShouldClose(game.window)) {
        if (glfwGetKey(game.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(game.window, true);

        ui::new_frame();
        {
            ui::robot_transform(robot);

            ui::programs(programs, &game.currently_recording, robot.rect.x,
                         robot.rect.y, robot.rotation);

            if(game.currently_recording != nullptr)
                ui::record(&game.currently_recording);
        }
        ui::render_frame();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render_map();
        game.render_robot(robot, glm::vec3(1, 0, 0));

        ui::render_gl_draw_data();

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    ui::destroy();

    return 0;
}
