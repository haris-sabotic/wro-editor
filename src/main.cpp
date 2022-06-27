#include "game.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main() {
    Game game;

    Texture texture = load_texture_from_file("res/images/map.png");

    Rect rect(0.0f, 0.0f, texture.width, texture.height);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(game.window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(game.window)) {
        if (glfwGetKey(game.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(game.window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render_texture_centered(
            texture.id,
            auto_fit_rect_in_rect(Rect(0, 0, game.win_width, game.win_height),
                                  rect),
            0.0f);

        game.render_rect_centered(glm::vec3(1, 0, 0), Rect(0, -1.5f, 130, 130),
                                  45.0f);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
