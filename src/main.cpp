#include "game.hpp"

int main() {
    Game game;

    Texture texture = load_texture_from_file("res/images/map.png");

    Rect rect(0.0f, 0.0f, texture.width, texture.height);

    while (!glfwWindowShouldClose(game.window)) {
        if (glfwGetKey(game.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(game.window, true);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render_texture(
            texture.id,
            auto_fit_rect_in_rect(Rect(0, 0, game.win_width, game.win_height),
                                  rect),
            0.0f);

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    return 0;
}
