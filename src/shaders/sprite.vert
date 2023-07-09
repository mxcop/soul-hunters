R"(#version 460 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texcoord;

out vec2 fTexcoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	fTexcoord = texcoord / 5.0f + 1.0f / 5.0f;
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
})"