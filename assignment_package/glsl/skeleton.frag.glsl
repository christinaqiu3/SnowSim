#version 150
// uniform sampler2D qt_Texture0;
// varying vec4 qt_TexCoord0;

// void main()
// {
//     gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0, 1.0);//texture2D(qt_Texture0, qt_TexCoord0.st);
// }


out vec4 out_Col;

void main()
{
    out_Col = vec4(0.0, 0.0, 0.0, 1.0);//fs_Col;
}
