#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments
uniform sampler2D u_ShadowTexture;

in vec4 fs_Col;
in vec2 fs_UV;
out vec4 out_Col;

void main()
{
    // Copy the color; there is no shading.
    out_Col = fs_Col;

    //CHANGED THIS TO RENDER THE SHADOW MAP TEXTURE
//    float z = texture(u_ShadowTexture, fs_UV).r;
//    out_Col = vec4(z,z,z,1);
}
