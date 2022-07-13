#include "constants.hpp"
#include "game.hpp"
#include "record.hpp"
#include "robot.hpp"
#include "ui.hpp"

int main() {
    Game game;

    ui::init(game.window);

    std::vector<Program> programs = {
        Program("Program 1",
                {
                    Instruction(InstructionType::MOVE_STRAIGHT, 1.0, 50.0),
                    Instruction(InstructionType::SPIN_TURN, 90, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, -2.1, 50.0),
                    Instruction(InstructionType::PIVOT_TURN_LEFT, -90, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, 1.5, 70.0),
                },
                0.0f, 0.0f, -45.0f),
        Program("Program 2",
                {
                    Instruction(InstructionType::MOVE_STRAIGHT, -1.0, 50.0),
                    Instruction(InstructionType::SPIN_TURN, 90, 40.0),
                    Instruction(InstructionType::MOVE_STRAIGHT, 2.5, 70.0),
                },
                200.0f, -300.0f, 27.0f),
        Program("Program 3",
                {
                    Instruction(InstructionType::SPIN_TURN, 180, 40.0),
                },
                0.0f, 0.0f, 0.0f),
    };

    std::unordered_map<InstructionType, float> motor_speeds;
    motor_speeds[InstructionType::MOVE_STRAIGHT] = 50.0f;
    motor_speeds[InstructionType::SPIN_TURN] = 40.0f;

    double current_frame = glfwGetTime();
    double last_frame = current_frame;

    while (!glfwWindowShouldClose(game.window)) {
        current_frame = glfwGetTime();
        game.delta_time = current_frame - last_frame;
        last_frame = current_frame;
        // printf("delta time: %f\n", delta_time);

        ui::new_frame();
        {
            ui::robot_transform(game.robot,
                                game.currently_recording != nullptr);

            ui::programs(programs, &game.currently_recording,
                         &game.currently_playing, game.robot);

            if (game.currently_recording != nullptr)
                ui::record(&game.currently_recording, game.robot, motor_speeds);
        }
        ui::render_frame();

        game.update();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render_map();
        game.render_robot();

        ui::render_gl_draw_data();

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    ui::destroy();

    return 0;
}
