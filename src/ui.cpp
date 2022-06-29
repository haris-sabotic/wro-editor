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

inline void display_single_program(Program &program) {
    ImGui::TextUnformatted(program.name.c_str());
    if (ImGui::BeginTable(program.name.c_str(), 5,
                          ImGuiTableFlags_BordersInner |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Instruction type");
        ImGui::TableSetupColumn("Number of rotations");
        ImGui::TableSetupColumn("Motor speed");
        ImGui::TableSetupColumn("");
        ImGui::TableHeadersRow();

        for (size_t row = 0; row < program.instructions.size(); row++) {
            ImGui::TableNextRow();
            for (int column = 0; column < 5; column++) {
                ImGui::TableSetColumnIndex(column);

                switch (column) {
                case 0:
                    ImGui::Text("%zu", row);
                    break;
                case 1:
                    switch (program.instructions[row].type) {
                    case InstructionType::MOVE_STRAIGHT:
                        ImGui::TextUnformatted("Move straight");
                        break;
                    case InstructionType::SPIN_TURN:
                        ImGui::TextUnformatted("Spin turn");
                        break;
                    case InstructionType::PIVOT_TURN_LEFT:
                        ImGui::TextUnformatted("Pivot turn left");
                        break;
                    case InstructionType::PIVOT_TURN_RIGHT:
                        ImGui::TextUnformatted("Pivot turn right");
                        break;
                    case InstructionType::NOOP:
                        ImGui::TextUnformatted("Empty");
                        break;
                    }
                    break;
                case 2:
                    ImGui::PushID(row * 3 + column); // assign unique id

                    ImGui::InputFloat("##v",
                                      &program.instructions[row].rotations);
                    ImGui::PopID();

                    break;
                case 3:
                    ImGui::PushID(row * 3 + column); // assign unique id

                    ImGui::InputFloat("##v", &program.instructions[row].speed);
                    ImGui::PopID();

                    break;
                case 4:
                    ImGui::PushID(row * 3 + column); // assign unique id

                    if (ImGui::Button("Del")) {
                        printf("Erase %zu\n", row);
                        program.instructions.erase(
                            program.instructions.begin() + row);
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Add before")) {
                        printf("Add before %zu\n", row);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Add after")) {
                        printf("Add after %zu\n", row);
                    }

                    ImGui::PopID();
                    break;
                }
            }
        }

        ImGui::EndTable();
    }
}

void ui::programs(std::vector<Program> &programs) {
    ImGui::Begin("Programs");
    {
        if(ImGui::Button("New program")) {
            printf("Create new program\n");
        }

        ImGui::Separator();
        
        for (auto &program : programs) {
            display_single_program(program);
            ImGui::Spacing();
        }
    }
    ImGui::End();
}
