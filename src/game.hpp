#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <GLFW/glfw3.h>
#include "util.hpp"
#include "robot.hpp"
#include "shader.hpp"

class Game {
    unsigned int quad_vao;
    unsigned int quad_vbo;
    unsigned int quad_ebo;

    std::unique_ptr<Shader> map_shader, robot_shader;

  public:
    int win_width = 1280;
    int win_height = 800;
    GLFWwindow *window;

    Rect map_rect;
    Texture map_texture;

    Game();
    ~Game();

    void on_resize_window();

    void render_map();
    void render_robot(const RobotData &robot_data, glm::vec3 color);

};

#endif // GAME_HPP
