#include "ui.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void ui::init(GLFWwindow *window) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void ui::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ui::new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ui::render_frame() {
    ImGui::Render();
}

void ui::render_gl_draw_data() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui::robot_transform(RobotData &robot_data) {
    ImGui::Begin("Robot transform");
    {
        ImGui::InputFloat("X", &robot_data.rect.x, 0.5f, 1.0f, "%.2f");
        ImGui::InputFloat("Y", &robot_data.rect.y, 0.5f, 1.0f, "%.2f");
        ImGui::Spacing();
        ImGui::InputFloat("Rotation", &robot_data.rotation, 0.5f, 1.0f, "%.2f");
    }
    ImGui::End();
}
