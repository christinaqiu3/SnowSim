#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform mat4 u_shadowMVP;

uniform vec4 u_Color;       // When drawing the cube instance, we'll set our uniform color to represent different block types.

uniform int u_Time;

uniform vec3 u_Light;

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

in vec2 vs_uv;              //fake UVs not sent in using VBOs

in vec4 vs_LL;

out vec4 fs_Pos;
out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;       // The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.

out vec2 fs_uv;
out vec4 fs_ll;

out float anim;
out vec4 fs_shadowPos;

//REPLACE THIS WIH THE CHANGING LIGHT POSITION IF WE DO DAY-NIGHT CYCLE
//const vec4 lightDir = normalize(vec4(0.5, 1, 0.75, 0));  // The direction of our virtual light, which is used to compute the shading of
                                        // the geometry in the fragment shader.

bool isWaterUV(vec2 uv) {
    vec2 d1 = abs(uv - vec2(0.8125f, 0.1875f));
    vec2 d2 = abs(uv - vec2(0.875, 0.1875f));
    vec2 d3 = abs(uv - vec2(0.875, 0.25));
    vec2 d4 = abs(uv - vec2(0.8125f, 0.25));

    float dP = 0.00390625;

    return length(d1) < dP || length(d2) < dP || length(d3) < dP || length(d4) < dP;

    //return uv == vec2(0.8125f, 0.1875f);
}

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

float WorleyNoise(vec2 uv)
{
    // Tile the space
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);

    float minDist = 1.0; // Minimum distance initialized to max.

    // Search all neighboring cells and this cell for their point
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            vec2 neighbor = vec2(float(x), float(y));

            // Random point inside current neighboring cell
            vec2 point = random2(uvInt + neighbor);

            // Animate the point
            point = 0.5 + 0.5 * sin(u_Time * 0.01 + 6.2831 * point); // 0 to 1 range

            // Compute the distance b/t the point and the fragment
            // Store the min dist thus far
            vec2 diff = neighbor + point - uvFract;
            float dist = length(diff);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

float fbm(vec3 uv) {
    float sum = 0;
    float freq = 4;
    float amp = 0.5;
    for(int i = 0; i < 8; i++) {
        sum += WorleyNoise(uv.xy * freq) * amp;
        freq *= 2;
        amp *= 0.5;
    }
    return sum;
}

vec3 calcDisplacedPos(vec3 v) {
    float waveDisplacement = 0.0;

        // Define multiple sine wave functions
        float amplitude1 = 0.5;
        float frequency1 = 0.5;
        float angle1 = 0.0;

        float amplitude2 = 0.3;
        float frequency2 = 1.0;
        float angle2 = 1.5;

        float amplitude3 = 0.2;
        float frequency3 = 2.0;
        float angle3 = 3.0;

        float amplitude4 = 0.4;
        float frequency4 = 0.8;
        float angle4 = 2.0;

        // Sum up contributions from different sine waves
        waveDisplacement += amplitude1 * sin(frequency1 * (v.x * cos(angle1) + v.z * sin(angle1)) + u_Time / 10.0);
        waveDisplacement += amplitude2 * sin(frequency2 * (v.x * cos(angle2) + v.z * sin(angle2)) + u_Time / 10.0);
        waveDisplacement += amplitude3 * sin(frequency3 * (v.x * cos(angle3) + v.z * sin(angle3)) + u_Time / 10.0);
        waveDisplacement += amplitude4 * sin(frequency4 * (v.x * cos(angle4) + v.z * sin(angle4)) + u_Time / 10.0);

        // Add FBM noise on top of existing displacement
        float fbmDisplacement = 0.5 * fbm(vec3(v.x * 0.5, v.z * 0.5, u_Time / 10.0));

        // Combine the displacements
        waveDisplacement += fbmDisplacement;
        waveDisplacement /= 4.0;
        waveDisplacement -= 0.5;

        return v + vec3(0.0, waveDisplacement, 0.0);
}


void main()
{
    fs_Pos = vs_Pos;

    fs_uv = vec2(vs_Col[0], vs_Col[1]);

    float ocVal = 1 - (vs_Col[3]*0.2);
    vec4 ocCol = vec4(ocVal, ocVal, ocVal, 1);

    fs_Col = ocCol;

    anim = vs_Col[2];

    mat3 invTranspose = mat3(u_ModelInvTr);

    vec4 modelposition = u_Model * vs_Pos;   // Temporarily store the transformed vertex positions for use below

    fs_LightVec = vec4(u_Light, 0.0);  // Compute the direction in which the light source lies


    if (isWaterUV(vs_Col.xy)) {

        vec3 pointA = calcDisplacedPos(vs_Pos.xyz);
        vec3 pointB = calcDisplacedPos(vs_Pos.xyz + vec3(1.0, 0.0, 0.0));
        vec3 pointC = calcDisplacedPos(vs_Pos.xyz + vec3(0.0, 0.0, 1.0));

        vec3 vecAB = pointB - pointA;
        vec3 vecAC = pointC - pointA;

        gl_Position = u_ViewProj * u_Model * vec4(pointA, 1.0);

        fs_Nor = vec4(normalize(cross(vecAC, vecAB)), 0.0);

    } else {
        fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);
        gl_Position = u_ViewProj * modelposition;
    }

    mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0,
                            0.5, 0.5, 0.5, 1.0);

    mat4 depthBiasMVP = biasMatrix * u_shadowMVP;

    //vec4 modelposition = u_Model * vec4(vs_Pos.xyz,1);   // Temporarily store the transformed vertex positions for use below

    fs_shadowPos = depthBiasMVP * vec4(vs_Pos.xyz,1);
    fs_ll = vs_LL;

}
