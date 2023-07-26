R"(#version 460 core
out vec4 out_color;

smooth in vec2 frag_pos;

uniform vec4 color;
uniform vec2 dir;
uniform vec2 angle;

float dist(vec2 p0, vec2 pf){return sqrt((pf.x-p0.x)*(pf.x-p0.x)+(pf.y-p0.y)*(pf.y-p0.y));}

void main()
{
    float d = dist(vec2(0.0, 0.0), frag_pos) * 2;
    out_color = color * vec4(1, 1, 1, 1.0 - d);

    vec2 frag_dir = normalize(frag_pos);
    float angular = acos(dot(frag_dir, dir));
    float angular_falloff = clamp(smoothstep(angle.x, angle.y, angular), 0.0, 0.4);

    out_color.a *= angular_falloff;
})"
