#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform mat4 u_Model;

uniform mat4 u_ViewProj;

uniform mat4 u_shadowMVP;

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

out vec4 fs_Pos;
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.


//REPLACE THIS WIH THE CHANGING LIGHT POSITION IF WE DO DAY-NIGHT CYCLE
//const vec4 lightDir = normalize(vec4(0.5, 1, 0.75, 0));  // The direction of our virtual light, which is used to compute the shading of
                                        // the geometry in the fragment shader.

void main()
{
    fs_Pos = vs_Pos;
    fs_Col = vs_Col;                         // Pass the vertex colors to the fragment shader for interpolation

    gl_Position =  u_shadowMVP * vs_Pos;// gl_Position is a built-in variable of OpenGL which is
                                        // used to render the final positions of the geometry's vertices
}

