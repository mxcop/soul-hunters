R"(#version 460 core
in vec2 fTexcoord;
out vec4 color;

uniform sampler2D sprite;

void main()
{
	color = texture(sprite, fTexcoord);
})"