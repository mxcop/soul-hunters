R"(#version 460 core
out vec4 out_color;

smooth in vec2 frag_pos;

uniform vec4 color;

float dist(vec2 p0, vec2 pf){return sqrt((pf.x-p0.x)*(pf.x-p0.x)+(pf.y-p0.y)*(pf.y-p0.y));}

void main()
{
    float d = dist(vec2(0.0, 0.0), frag_pos)* 2;
    out_color = color * vec4(1, 1, 1, 1.0 - d);
	// out_color.a = 1.0 - d;

	// out_color = color;
})"
