R"(#version 460 core
in vec2 position;

smooth out vec2 frag_pos;
flat out vec2 origin;

uniform mat4 model;
uniform mat4 projection;

void main() {
    /* Origin point of the current triangle (flat mapped) */
    origin = position;
    /* Fragment position (smooth mapped) */
    frag_pos = position;

    gl_Position = projection * model * vec4(position, 0.0, 1.0);
})"