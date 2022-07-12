#include "util.hpp"
#include "glm/geometric.hpp"
#include <cstddef>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

Texture load_texture_from_file(std::string_view path) {
    unsigned int id;
    glGenTextures(1, &id);

    int width, height, channels;
    unsigned char *data = stbi_load(path.data(), &width, &height, &channels, 0);
    GLenum format = GL_RGB;
    if (data) {
        if (channels == 1)
            format = GL_RED;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        fprintf(stderr, "Failed to load texture %s\n", path.data());
    }
    stbi_image_free(data);

    Texture t;
    t.width = width;
    t.height = height;
    t.id = id;
    return t;
}

Rect auto_fit_rect_in_rect(Rect out_rect, Rect in_rect) {
    double in_rect_aspect = in_rect.width / in_rect.height;
    Rect result_rect;

    result_rect.x = 0.0f;
    result_rect.y = 0.0f;

    /// Fit by width first
    result_rect.width = out_rect.width;
    result_rect.height = result_rect.width / in_rect_aspect;

    // If fitting by width wasn't right, fit by height instead
    if (result_rect.height > out_rect.height) {
        result_rect.height = out_rect.height;
        result_rect.width = result_rect.height * in_rect_aspect;
    }

    return result_rect;
}

void find_rectangle_front_vertices(const Rect &rect, float rotation,
                                   glm::vec2 &vertl, glm::vec2 &vertr) {
    vertl.x = rect.x, vertl.y = rect.y;
    vertr.x = rect.x, vertr.y = rect.y;

    float c = glm::cos(glm::radians(rotation));
    float s = glm::sin(glm::radians(rotation));

    float c1x = c * rect.width / 2;
    float c1y = s * rect.width / 2;
    float c2x = s * rect.height / 2;
    float c2y = c * rect.height / 2;

    vertl.x += c2x;
    vertl.y -= c2y;
    vertl.x -= c1x;
    vertl.y -= c1y;

    vertr.x += c2x;
    vertr.y -= c2y;
    vertr.x += c1x;
    vertr.y += c1y;
}

bool get_line_intersection(const Line &line1, const Line &line2,
                           glm::vec2 &intersection) {
    /// https://stackoverflow.com/a/1968345

    float s1_x, s1_y, s2_x, s2_y;
    s1_x = line1.p1.x - line1.p0.x;
    s1_y = line1.p1.y - line1.p0.y;
    s2_x = line2.p1.x - line2.p0.x;
    s2_y = line2.p1.y - line2.p0.y;

    float s, t;
    s = (-s1_y * (line1.p0.x - line2.p0.x) + s1_x * (line1.p0.y - line2.p0.y)) /
        (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (line1.p0.y - line2.p0.y) - s2_y * (line1.p0.x - line2.p0.x)) /
        (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        // Collision detected
        intersection.x = line1.p0.x + (t * s1_x);
        intersection.y = line1.p0.y + (t * s1_y);
        return true;
    }

    return false; // No collision
}
