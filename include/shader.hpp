#ifndef SHADER_HPP
#define SHADER_HPP

#include "glm/glm.hpp"
#include <string_view>

struct Shader {
    // OpenGL id you get from glCreateProgram
    unsigned int id;

    Shader(unsigned int id);
    Shader(std::string_view vertex_shader_path,
           std::string_view frag_shader_path);

    ~Shader();

    // Since everything is entirely 2D, this just calculates the projection
    // matrix with orthogonal projection.
    // Used for mapping screen coordinates from the top left to the format
    // opengl expects
    void set_projection_matrix(int win_width, int win_height);

    void use() const;

    /// Functions for setting uniforms
    void set_i(const char *name, int value) const;
    void set_f(const char *name, float value) const;
    void set_vec3(const char *name, float x, float y, float z) const;
    void set_vec3(const char *name, glm::vec3 vec) const;
    void set_mat4(const char *name, glm::mat4 value) const;
};

#endif // SHADER_HPP
