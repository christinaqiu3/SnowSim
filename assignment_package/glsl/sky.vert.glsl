#version 150

in vec4 vs_Pos;

void main()
{
    //just passthrough
    gl_Position = vs_Pos;
}
