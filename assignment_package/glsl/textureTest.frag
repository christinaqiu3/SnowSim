#version 150
uniform sampler2D u_ShadowTexture;

in vec4 fs_Pos;
in vec2 fs_UV;

out vec4 out_Col;

void main()
{
    out_Col = vec4(1);
}
