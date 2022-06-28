#include <glad/glad.h>
#include "game.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <cstdio>

void Game::on_resize_window() {
    map_rect = auto_fit_rect_in_rect(Rect(0.0f, 0.0f, win_width, win_height),
                                     map_rect);

    glm::mat4 projection =
        glm::ortho(-(float)win_width / 2, (float)win_width / 2,
                   (float)win_height / 2, -(float)win_height / 2, -1.0f, 1.0f);

    map_shader->use();
    map_shader->set_mat4("projection", projection);

    robot_shader->use();
    robot_shader->set_mat4("projection", projection);
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
            game->on_resize_window();

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

    map_texture = load_texture_from_file("res/images/map.png");
    map_rect = Rect(0.0f, 0.0f, map_texture.width, map_texture.height);

    map_shader = std::make_unique<Shader>(
        Shader("shaders/texture.vs", "shaders/texture.fs"));
    robot_shader = std::make_unique<Shader>(
        Shader("shaders/color.vs", "shaders/color.fs"));
    on_resize_window();
}

Game::~Game() {
    glDeleteVertexArrays(1, &quad_vao);
    glDeleteBuffers(1, &quad_vbo);
    glDeleteBuffers(1, &quad_ebo);

    map_shader->destroy();

    glfwTerminate();
}

void Game::render_map() {
    map_shader->use();
    map_shader->set_i("texture_id", map_texture.id);

    /// apply transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(map_rect.x, map_rect.y, 0.0f));
    model = glm::scale(model, glm::vec3(map_rect.width, map_rect.height, 1.0f));

    map_shader->set_mat4("model", model);

    glBindVertexArray(quad_vao);
    glActiveTexture(GL_TEXTURE0 + map_texture.id);
    glBindTexture(GL_TEXTURE_2D, map_texture.id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Game::render_robot(const RobotData &robot_data, glm::vec3 color) {
    robot_shader->use();
    robot_shader->set_vec3("color", color);

    Rect rect = robot_data.screen_rect(map_rect, map_texture.width,
                                                  map_texture.height);

    /// apply transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rect.x, rect.y, 0.0f));
    model = glm::rotate(model, glm::radians(robot_data.rotation),
                        glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(
        model, glm::vec3(rect.width, rect.height, 1.0f));

    robot_shader->set_mat4("model", model);

    glBindVertexArray(quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
