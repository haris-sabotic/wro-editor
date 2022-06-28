#include "game.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main() {
    Game game;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(game.window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Texture map_texture = load_texture_from_file("res/images/map.png");
    Rect map_rect(0.0f, 0.0f, map_texture.width, map_texture.height);

    Rect robot_rect(0.0f, 0.0f, 100.0f, 130.0f);
    float robot_rotation = -45.0f;

    while (!glfwWindowShouldClose(game.window)) {
        if (glfwGetKey(game.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(game.window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Robot transform");
        {
            ImGui::InputFloat("X", &robot_rect.x, 0.5f, 1.0f, "%.2f");
            ImGui::InputFloat("Y", &robot_rect.y, 0.5f, 1.0f, "%.2f");
            ImGui::Spacing();
            ImGui::InputFloat("Rotation", &robot_rotation, 0.5f, 1.0f, "%.2f");
        }
        ImGui::End();

        ImGui::Render();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render_texture_centered(
            map_texture.id,
            auto_fit_rect_in_rect(Rect(0, 0, game.win_width, game.win_height),
                                  map_rect),
            0.0f);

        game.render_rect_centered(glm::vec3(1, 0, 0), robot_rect,
                                  robot_rotation);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
