#include "game.hpp"
#include "ui.hpp"

int main() {
    Game game;

    ui::init(game.window);

    Texture map_texture = load_texture_from_file("res/images/map.png");
    Rect map_rect(0.0f, 0.0f, map_texture.width, map_texture.height);

    RobotData robot(Rect(0.0f, 0.0f, 100.0f, 130.0f), -45.0f);

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

        game.render_texture_centered(
            map_texture.id,
            auto_fit_rect_in_rect(Rect(0, 0, game.win_width, game.win_height),
                                  map_rect),
            0.0f);

        game.render_rect_centered(glm::vec3(1, 0, 0), robot.rect,
                                  robot.rotation);

        ui::render_gl_draw_data();

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    ui::destroy();

    return 0;
}
