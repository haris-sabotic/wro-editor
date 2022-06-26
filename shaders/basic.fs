#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_id;


void main() {
    FragColor = texture(texture_id, TexCoord);
}