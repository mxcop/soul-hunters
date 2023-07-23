R"(#version 460 core
layout (location = 0) in vec2 position;

smooth out vec2 frag_pos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    frag_pos = position;
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
})"