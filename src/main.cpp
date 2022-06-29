#include "game.hpp"
#include "ui.hpp"

int main() {
    Game game;

    ui::init(game.window);

    RobotData robot(Rect(0.0f, 0.0f, 170.0f, 190.0f), -45.0f);

    while (!glfwWindowShouldClose(game.window)) {
        if (glfwGetKey(game.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(game.window, true);

        ui::new_frame();
        {
            ui::robot_transform(robot);
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
