#include "util.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>


Rect::Rect(float x, float y, float width, float height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

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

        printf("%d\n", channels);

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
    Rect result_rect(0.0f, 0.0f, 0.0f, 0.0f);

    /// Fit by width first
    result_rect.width = out_rect.width;
    result_rect.height = result_rect.width / in_rect_aspect;
    result_rect.y = out_rect.height/2 - result_rect.height/2;

    // If fitting by width wasn't right, fit by height instead
    if(result_rect.height > out_rect.height) {
        result_rect.y = 0.0f;

        result_rect.height = out_rect.height;
        result_rect.width = result_rect.height * in_rect_aspect;
        result_rect.x = out_rect.width/2 - result_rect.width/2;
    }

    return result_rect;
}
