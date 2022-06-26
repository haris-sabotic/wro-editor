#include <glad/glad.h>
#include "game.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <cstdio>

void Game::reset_projection_matrices() {
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::ortho(0.0f, (float)win_width, (float)win_height, 0.0f,
                            -1.0f, 1.0f);

    texture_shader->use();
    texture_shader->set_mat4("projection", projection);

    color_shader->use();
    color_shader->set_mat4("projection", projection);
}

Game::Game() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(win_width, win_height, "WRO Editor", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window!\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // store reference to this inside the glfw window
    // this way, I can access this inside the FrameBufferSizeCallback, since it
    // takes in a pointer to the glfw window
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            Game *game = (Game *)glfwGetWindowUserPointer(window);
            game->win_width = width;
            game->win_height = height;
            game->reset_projection_matrices();

            glViewport(0, 0, width, height);
        });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD!\n");
        glfwTerminate();
        exit(1);
    }

    float quad_vertices[] = {
        // positions   // texture coordinates
        0.5f,  0.5f,  1.0f, 1.0f, // top right
        0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, // top left
    };
    unsigned int quad_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &quad_vao);
    glGenBuffers(1, &quad_vbo);
    glGenBuffers(1, &quad_ebo);

    glBindVertexArray(quad_vao);

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture_shader = std::make_unique<Shader>(
        Shader("shaders/texture.vs", "shaders/texture.fs"));
    color_shader = std::make_unique<Shader>(
        Shader("shaders/color.vs", "shaders/color.fs"));
    reset_projection_matrices();
}

Game::~Game() {
    glDeleteVertexArrays(1, &quad_vao);
    glDeleteBuffers(1, &quad_vbo);
    glDeleteBuffers(1, &quad_ebo);

    texture_shader->destroy();

    glfwTerminate();
}

void Game::render_texture_centered(unsigned int id, Rect rect, float rotation) {
    texture_shader->use();
    texture_shader->set_i("texture_id", id);

    /// apply transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0.0f));
    model =
        glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 1.0f));

    texture_shader->set_mat4("model", model);

    glBindVertexArray(quad_vao);
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Game::render_texture(unsigned int id, Rect rect, float rotation) {
    render_texture_centered(id,
                            Rect(rect.x + rect.width / 2,
                                 rect.y + rect.height / 2, rect.width,
                                 rect.height),
                            rotation);
}

void Game::render_rect_centered(glm::vec3 color, Rect rect, float rotation) {
    color_shader->use();
    color_shader->set_vec3("color", color);

    /// apply transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0.0f));
    model =
        glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(rect.width, rect.height, 1.0f));

    color_shader->set_mat4("model", model);

    glBindVertexArray(quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Game::render_rect(glm::vec3 color, Rect rect, float rotation) {
    render_rect_centered(color,
                         Rect(rect.x + rect.width / 2, rect.y + rect.height / 2,
                              rect.width, rect.height),
                         rotation);
}
