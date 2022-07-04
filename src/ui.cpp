#include "ui.hpp"
#include "imgui_internal.h"
#include "record.hpp"
#include "robot.hpp"
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

void ui::render_frame() { ImGui::Render(); }

void ui::render_gl_draw_data() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui::robot_transform(RobotData &robot_data, bool disabled) {
    ImGui::Begin("Robot transform");
    {
        if (disabled) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                                ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::InputFloat("X", &robot_data.rect.x, 0.5f, 1.0f);
        ImGui::InputFloat("Y", &robot_data.rect.y, 0.5f, 1.0f);
        ImGui::Spacing();
        ImGui::InputFloat("Rotation", &robot_data.rotation, 0.5f, 1.0f);

        if (disabled) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
    }
    ImGui::End();
}

// returns false if the program needs to be deleted(button press)
inline bool display_single_program(Program &program,
                                   Instruction **currently_recording,
                                   RobotData &robot_data) {
    ImGui::TextUnformatted(program.name.c_str());

    // Create an empty instruction and start recording
    if (ImGui::Button("Add instruction")) {
        program.instructions.push_back(Instruction());
        *currently_recording =
            &program.instructions[program.instructions.size() - 1];
        transform_robot_until_instruction(robot_data, program,
                                          program.instructions.size() - 1);
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete program")) {
        return false;
    }

    if (ImGui::BeginTable(program.name.c_str(), 6,
                          ImGuiTableFlags_BordersInner |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Instruction type");
        ImGui::TableSetupColumn("Distance(rotations)");
        ImGui::TableSetupColumn("Angle(degrees)");
        ImGui::TableSetupColumn("Motor speed");
        ImGui::TableSetupColumn("");
        ImGui::TableHeadersRow();

        for (size_t row = 0; row < program.instructions.size(); row++) {
            ImGui::TableNextRow();

            // if currently recording, disable every element(can't edit or
            // add/remove other instructions)
            bool added_flag = false;
            if (*currently_recording != nullptr &&
                *currently_recording != &program.instructions[row]) {
                added_flag = true;
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                                    ImGui::GetStyle().Alpha * 0.5f);
            }

            for (int column = 0; column < 6; column++) {
                ImGui::TableSetColumnIndex(column);

                switch (column) {
                case 0:
                    // display instruction id
                    ImGui::Text("%zu", row);
                    break;
                case 1:
                    // display instruction type
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
                    // if the type is move-straight, then display the
                    // instruction count(distance passed), otherwise leave empty
                    if (program.instructions[row].type ==
                        InstructionType::MOVE_STRAIGHT) {
                        ImGui::PushID(row * 3 + column); // assign unique id

                        ImGui::InputFloat("##v",
                                          &program.instructions[row].count);
                        ImGui::PopID();
                    }

                    break;
                case 3:
                    // leave empty if the instruction type is move-straight,
                    // otherwise display instruction count(degrees rotated)
                    if (program.instructions[row].type !=
                        InstructionType::MOVE_STRAIGHT) {
                        ImGui::PushID(row * 3 + column); // assign unique id

                        ImGui::InputFloat("##v",
                                          &program.instructions[row].count);
                        ImGui::PopID();
                    }

                    break;
                case 4:
                    // display motor speed
                    ImGui::PushID(row * 3 + column); // assign unique id

                    ImGui::InputFloat("##v", &program.instructions[row].speed);
                    ImGui::PopID();

                    break;
                case 5:
                    /// Instruction control buttons(erase, add new instruction
                    /// before this one, add new instruction after this one)

                    ImGui::PushID(row * 3 + column); // assign unique id

                    if (ImGui::Button("Del")) {
                        printf("Erase %zu\n", row);

                        if (*currently_recording == &program.instructions[row])
                            *currently_recording = nullptr;

                        program.instructions.erase(
                            program.instructions.begin() + row);
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Add before")) {
                        printf("Add before %zu\n", row);

                        program.instructions.emplace(
                            program.instructions.begin() + row);
                        *currently_recording = &program.instructions[row];
                        transform_robot_until_instruction(robot_data, program,
                                                          row);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Add after")) {
                        printf("Add after %zu\n", row);

                        program.instructions.emplace(
                            program.instructions.begin() + row + 1);
                        *currently_recording = &program.instructions[row + 1];
                        transform_robot_until_instruction(robot_data, program,
                                                          row + 1);
                    }

                    ImGui::PopID();
                    break;
                }
            }

            if (added_flag) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        ImGui::EndTable();
    }

    return true;
}

void ui::programs(std::vector<Program> &programs,
                  Instruction **currently_recording, RobotData &robot_data) {
    static char buf[128];

    ImGui::Begin("Programs");
    {
        // Adding a new program
        ImGui::InputText("##program name", buf, 128);
        ImGui::SameLine();
        if (ImGui::Button("New program")) {
            if (strlen(buf) > 0) {
                printf("Create new program, name: %s\n", buf);

                std::vector<Instruction> instructions = {Instruction()};
                programs.push_back(Program(std::string(buf), instructions,
                                           robot_data.rect.x, robot_data.rect.y,
                                           robot_data.rotation));
                *currently_recording =
                    &programs[programs.size() - 1].instructions[0];
            }
        }

        ImGui::Separator();

        // display every program
        int i = 0;
        for (auto &program : programs) {
            ImGui::PushID(i * 3 + 500); // assign unique id

            if (!display_single_program(program, currently_recording,
                                        robot_data)) {
                programs.erase(programs.begin() + i);
            }

            ImGui::Spacing();

            ImGui::PopID();
            i++;
        }
    }
    ImGui::End();
}

void ui::record(Instruction **currently_recording, RobotData &robot_data,
            std::unordered_map<InstructionType, float> &motor_speeds) {
    ImGui::Begin("Recording...");
    {
        /// If an instruction type hasn't been selected yet, disable the button
        /// to stop recording
        if ((*currently_recording)->type == InstructionType::NOOP) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                                ImGui::GetStyle().Alpha * 0.5f);
        }
        if (ImGui::Button("Stop recording")) {
            motor_speeds[(*currently_recording)->type] =
                (*currently_recording)->speed;
            transform_robot_per_instruction(robot_data, *currently_recording);
            *currently_recording = nullptr;
            ImGui::End();
            return;
        }
        if ((*currently_recording)->type == InstructionType::NOOP) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        /// Radio buttons to select the type - maybe switch to a list box?
        if (ImGui::RadioButton("Move straight",
                               (*currently_recording)->type ==
                                   InstructionType::MOVE_STRAIGHT)) {
            (*currently_recording)->type = InstructionType::MOVE_STRAIGHT;
            (*currently_recording)->speed =
                motor_speeds[InstructionType::MOVE_STRAIGHT];
        }
        if (ImGui::RadioButton("Spin turn", (*currently_recording)->type ==
                                                InstructionType::SPIN_TURN)) {
            (*currently_recording)->type = InstructionType::SPIN_TURN;
            (*currently_recording)->speed =
                motor_speeds[InstructionType::SPIN_TURN];
        }
        if (ImGui::RadioButton("Pivot turn left",
                               (*currently_recording)->type ==
                                   InstructionType::PIVOT_TURN_LEFT)) {
            (*currently_recording)->type = InstructionType::PIVOT_TURN_LEFT;
            (*currently_recording)->speed =
                motor_speeds[InstructionType::PIVOT_TURN_LEFT];
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Pivot turn right",
                               (*currently_recording)->type ==
                                   InstructionType::PIVOT_TURN_RIGHT)) {
            (*currently_recording)->type = InstructionType::PIVOT_TURN_RIGHT;
            (*currently_recording)->speed =
                motor_speeds[InstructionType::PIVOT_TURN_RIGHT];
        }

        ImGui::Spacing();

        std::string count_label;
        // don't display a label if no instruction's been selected
        if ((*currently_recording)->type == InstructionType::NOOP)
            count_label = "##empty";
        else if ((*currently_recording)->type == InstructionType::MOVE_STRAIGHT)
            count_label = "Distance(rotations)";
        else // every other instruction is rotation-related
            count_label = "Angle(degrees)";
        ImGui::InputFloat(count_label.c_str(), &(*currently_recording)->count, 1.0f, 2.0f);

        ImGui::InputFloat("Motor speed", &(*currently_recording)->speed, 1.0f, 2.0f);
    }
    ImGui::End();
}
