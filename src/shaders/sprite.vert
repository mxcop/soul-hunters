R"(#version 460 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texcoord;

out vec2 fTexcoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	fTexcoord = texcoord;
    gl_Position = projection * model * vec4(position, 0.0, 1.0);
})"