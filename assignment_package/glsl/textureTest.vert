#version 150
uniform mat4 u_Model;

uniform mat4 u_ModelInvTr;

uniform mat4 u_ViewProj;

in vec4 vs_Pos;
in vec2 vs_UV;

out vec4 fs_Pos;
out vec2 fs_UV;

void main()
{
    fs_Pos = vs_Pos;
    fs_UV = vs_UV;

    vec4 modelposition = u_Model * vs_Pos;

    gl_Position = u_ViewProj * modelposition;
}
