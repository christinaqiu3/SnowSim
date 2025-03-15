#version 330
// noOp.vert.glsl:
// A fragment shader used for post-processing that simply reads the
// image produced in the first render pass by the surface shader
// and outputs it to the frame buffer


in vec2 fs_UV;

in vec4 fs_Pos;

out vec4 color;

uniform sampler2D u_RenderedTexture;

uniform sampler2D u_ShadowTexture;

uniform vec3 u_PlayerPos;

uniform ivec2 u_Dimensions;
//BLOOM kernel
const float kernelB[121] = float[](0.006849, 0.007239, 0.007559, 0.007795, 0.007941, 0.00799, 0.007941, 0.007795, 0.007559, 0.007239, 0.006849, 0.007239, 0.007653, 0.00799, 0.00824, 0.008394, 0.008446, 0.008394, 0.00824, 0.00799, 0.007653, 0.007239, 0.007559, 0.00799, 0.008342, 0.008604, 0.008764, 0.008819, 0.008764, 0.008604, 0.008342, 0.00799, 0.007559, 0.007795, 0.00824, 0.008604, 0.008873, 0.009039, 0.009095, 0.009039, 0.008873, 0.008604, 0.00824, 0.007795, 0.007941, 0.008394, 0.008764, 0.009039, 0.009208, 0.009265, 0.009208, 0.009039, 0.008764, 0.008394, 0.007941, 0.00799, 0.008446, 0.008819, 0.009095, 0.009265, 0.009322, 0.009265, 0.009095, 0.008819, 0.008446, 0.00799, 0.007941, 0.008394, 0.008764, 0.009039, 0.009208, 0.009265, 0.009208, 0.009039, 0.008764, 0.008394, 0.007941, 0.007795, 0.00824, 0.008604, 0.008873, 0.009039, 0.009095, 0.009039, 0.008873, 0.008604, 0.00824, 0.007795, 0.007559, 0.00799, 0.008342, 0.008604, 0.008764, 0.008819, 0.008764, 0.008604, 0.008342, 0.00799, 0.007559, 0.007239, 0.007653, 0.00799, 0.00824, 0.008394, 0.008446, 0.008394, 0.00824, 0.00799, 0.007653, 0.007239, 0.006849, 0.007239, 0.007559, 0.007795, 0.007941, 0.00799, 0.007941, 0.007795, 0.007559, 0.007239, 0.006849);

//BLUR kernel
const float kernelG[121] = float[](0.006849, 0.007239, 0.007559, 0.007795, 0.007941, 0.00799, 0.007941, 0.007795, 0.007559, 0.007239, 0.006849, 0.007239, 0.007653, 0.00799, 0.00824, 0.008394, 0.008446, 0.008394, 0.00824, 0.00799, 0.007653, 0.007239, 0.007559, 0.00799, 0.008342, 0.008604, 0.008764, 0.008819, 0.008764, 0.008604, 0.008342, 0.00799, 0.007559, 0.007795, 0.00824, 0.008604, 0.008873, 0.009039, 0.009095, 0.009039, 0.008873, 0.008604, 0.00824, 0.007795, 0.007941, 0.008394, 0.008764, 0.009039, 0.009208, 0.009265, 0.009208, 0.009039, 0.008764, 0.008394, 0.007941, 0.00799, 0.008446, 0.008819, 0.009095, 0.009265, 0.009322, 0.009265, 0.009095, 0.008819, 0.008446, 0.00799, 0.007941, 0.008394, 0.008764, 0.009039, 0.009208, 0.009265, 0.009208, 0.009039, 0.008764, 0.008394, 0.007941, 0.007795, 0.00824, 0.008604, 0.008873, 0.009039, 0.009095, 0.009039, 0.008873, 0.008604, 0.00824, 0.007795, 0.007559, 0.00799, 0.008342, 0.008604, 0.008764, 0.008819, 0.008764, 0.008604, 0.008342, 0.00799, 0.007559, 0.007239, 0.007653, 0.00799, 0.00824, 0.008394, 0.008446, 0.008394, 0.00824, 0.00799, 0.007653, 0.007239, 0.006849, 0.007239, 0.007559, 0.007795, 0.007941, 0.00799, 0.007941, 0.007795, 0.007559, 0.007239, 0.006849);


void main()
{

//    BLOOM
//    vec4 colAverage = texture(u_RenderedTexture, fs_UV);
//    for (int i = -5; i<6; i++) {
//        for (int j = -5; j<6; j++) {
//            float x = fs_UV.x + float(i)/u_Dimensions.x;
//            float y = fs_UV.y + float(j)/u_Dimensions.y;
//            vec4 baseCol = texture(u_RenderedTexture, vec2(x,y));
//            if (dot(baseCol.rgb, vec3(0.2126, 0.7152, 0.0722)) >= 0.8) {
//                colAverage += baseCol * kernelB[(i+5) + (j+5)*11] * 0.01;
//            }
//        }
//    }
//    color = vec4(colAverage.rgb, 1);



    //LENSE FLARE

//    vec2 uv = vec2(1.0) - fs_UV; // flip the texture coordinates
//    vec3 ret = vec3(0.0);
//    vec2 ghostVec = (vec2(0.5) - uv) * 3;
//    for (int i = 0; i < 6; ++i) {
//            vec2 suv = fract(uv + ghostVec * vec2(i));
//            float d = distance(suv, vec2(0.5));
//            float weight = 1.0 - smoothstep(0.0, 0.75, d); // reduce contributions from samples at the screen edge
//            vec3 s = texture(u_RenderedTexture, fs_UV).rgb;
//            s = max(s - vec3(0.5), vec3(0.0));
//            ret += s * weight;
//    }
//    color = texture(u_RenderedTexture, fs_UV) + vec4(ret,1);


    //GAUSSIAN BLUR

    vec4 colAverage = texture(u_RenderedTexture, fs_UV);

    for (int i = -5; i<6; i++) {
        for (int j = -5; j<6; j++) {
            float x = fs_UV.x + float(i)/u_Dimensions.x;
            float y = fs_UV.y + float(j)/u_Dimensions.y;
            colAverage += texture(u_RenderedTexture, vec2(x,y)) * kernelG[(i+5) + (j+5)*11] * 0.08;
        }
    }
    color = colAverage;
    color = texture(u_RenderedTexture, fs_UV);


    // For the purposes of rendering the shadow map, maybe just
    // set:
//     float z = texture(u_RenderedTexture, fs_UV).r;
//     color = vec4(z,z,z,1);
    //color = texture(u_ShadowTexture, fs_UV);
}






