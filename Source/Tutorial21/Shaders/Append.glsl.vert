#version 430 core

layout (location = 0) in vec4 position;

uniform mat4 ModelViewProjection;

out gl_PerVertex
{
    vec4 gl_Position;
};

out VS_OUT
{
    vec4 pos;
    vec4 color;
} vs_out;

void main(void)
{
    vec4 p = ModelViewProjection * position;

    gl_Position = p;
    vs_out.color = vec4(1.0);
    vs_out.pos = p / p.w;
}
