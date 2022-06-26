#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <GLFW/glfw3.h>
#include "util.hpp"
#include "shader.hpp"

class Game {
    unsigned int quad_vao;
    unsigned int quad_vbo;
    unsigned int quad_ebo;

    std::unique_ptr<Shader> texture_shader, color_shader;

  public:
    int win_width = 1280;
    int win_height = 800;
    GLFWwindow *window;

    Game();
    ~Game();

    void reset_projection_matrices();

    void render_texture_centered(unsigned int id, Rect rect, float rotation);
    void render_texture(unsigned int id, Rect rect, float rotation);

    void render_rect_centered(glm::vec3 color, Rect rect, float rotation);
    void render_rect(glm::vec3 color, Rect rect, float rotation);
};

#endif // GAME_HPP
