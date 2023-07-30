R"(#version 460 core
in vec2 fTexcoord;
out vec4 color;

/* Animation uniforms */
uniform int frames = 1;
uniform int frame = 0;

uniform sampler2D sprite;

void main()
{
	//color = texture(sprite, fTexcoord);
	color = texture(sprite, vec2((frame + fTexcoord.x) / frames, 1.0 - fTexcoord.y));
})"