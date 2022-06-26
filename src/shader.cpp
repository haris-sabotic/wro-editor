#include <glad/glad.h>
#include "shader.hpp"
#include <fstream>
#include "glm/gtc/type_ptr.hpp"

// Compiles individual shaders and reports errors if there are any
unsigned int create_shader(std::string_view shader_path, GLenum shader_type) {
    /// load file content into shader_src
    std::ifstream ifs(shader_path.data());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    const char *shader_src = content.c_str();

    /// Create and compile shader
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    /// Check for errors
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    printf("Shader compilation %s - %s\n", success ? "SUCCEEDED" : "FAILED",
           shader_path.data());
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("%s", info_log);
    }

    return shader;
}

Shader::Shader(unsigned int id) {
    this->id = id;
}

Shader::Shader(std::string_view vertex_shader_path,
               std::string_view frag_shader_path) {
    /// Create shader program and link it with the vertex and fragment shader
    id = glCreateProgram();
    glAttachShader(id, create_shader(vertex_shader_path, GL_VERTEX_SHADER));
    glAttachShader(id, create_shader(frag_shader_path, GL_FRAGMENT_SHADER));
    glLinkProgram(id);

    /// Check for errors
    int success;
    char info_log[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    printf("Linking shader program %s\n", success ? "SUCCEEDED\n" : "FAILED");
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        printf("%s\n\n", info_log);
    }
}

Shader::~Shader() {
    destroy();
}

void Shader::destroy() {
    glDeleteProgram(id);
}

void Shader::set_projection_matrix(int win_width, int win_height) {
    glm::mat4 projection = glm::mat4(1.0f);
    // top-left = 0, 0
    // bottom-right = width, height
    projection =
        glm::ortho(0.0f, (float)win_width, (float)win_height, 0.0f, -1.0f, 1.0f);

    this->use();
    this->set_mat4("projection", projection);
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::set_i(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(id, name), value);
}
void Shader::set_f(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}
void Shader::set_vec3(const char *name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}
void Shader::set_mat4(const char *name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE,
                       glm::value_ptr(value));
}
void Shader::set_vec3(const char *name, glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(id, name), vec.x, vec.y, vec.z);
}
