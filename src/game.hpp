#ifndef GAME_HPP
#define GAME_HPP

#include <cstddef>
#include <string_view>
#include <vector>
#include <GLFW/glfw3.h>
#include "util.hpp"
#include "shader.hpp"

class Game {
    unsigned int quad_vao;
    unsigned int quad_vbo;
    unsigned int quad_ebo;

    std::vector<Shader> m_shaders;

  public:
    int WIN_WIDTH = 1280;
    int WIN_HEIGHT = 800;
    GLFWwindow *window;

    Game();
    ~Game();

    void reset_projection_matrices();

    // create shader, add it to m_shaders and set its projection matrix
    size_t create_shader(std::string_view vertex_shader_path, std::string_view fragment_shader_path);
    // access shader in m_shaders
    const Shader& shader(size_t id);


    void render_texture_centered(unsigned int id, const Shader &shader, Rect rect,
                                 float rotation);
    void render_texture(unsigned int id, const Shader &shader, Rect rect,
                        float rotation);

    void render_texture_centered(unsigned int id, size_t shader, Rect rect,
                                 float rotation);
    void render_texture(unsigned int id, size_t shader, Rect rect,
                        float rotation);
};

#endif // GAME_HPP
