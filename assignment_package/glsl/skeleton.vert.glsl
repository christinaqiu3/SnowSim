#version 150
// attribute vec4 qt_Vertex;
// attribute vec4 qt_MultiTexCoord0;
// uniform mat4 qt_ModelViewProjectionMatrix;
// varying vec4 qt_TexCoord0;

// in vec4 vs_Pos;
// in vec4 vs_Col;

// in vec2 in_weights;
// in ivec2 in_jointIDs;

// uniform mat4 u_Model;
// uniform mat4 u_ViewProj;

// uniform mat4 ua_jointMats[100];
// uniform mat4 ua_bindMats[100];

// out vec4 fs_Nor;

// out vec4 fs_Col;

// in vec3 K = (0,0,0);
// in vec3 B = (0,0,1);
// in vec3 G = (0,1,0);
// in vec3 Y = (1,1,0);
// in vec3 R = (1,0,0);
// in vec3 W = (1,1,1);

// vec3 heatMap(float t) {
//     if(t < 0.167) {
//         return mix(K, B, t/.167);
//     }
//     else if(t < 0.333) {
//         return mix(B, G, (t-.167)/.167);
//     }
//     else if(t < 0.5) {
//         return mix(G, Y, (t-.333)/.167);
//     }
//     else if(t < 0.667) {
//         return mix(Y, R, (t-.5)/.167);
//     }
//     else {
//         return mix(R, W, (t-.667)/.167);
//     }

// }

void main()
{
    // int firstJointID = in_jointIDs[0];
    // vec4 posLocalToFirstJoint = ua_bindMats[firstJointID] * vs_Pos;
    // vec4 firstDeformedPos = ua_jointMats[firstJointID] * posLocalToFirstJoint;
    // int secondJointID = in_jointIDs[1];
    // vec4 posLocalToFirstJoint = ua_bindMats[secondJointID] * vs_Pos;
    // vec4 secondDeformedPos = ua_jointMats[secondJointID] * posLocalToSecondJoint;


    // gl_Position = u_ViewProj * (in_weights[0] * firstDeformedPos + in_weights[1] * secondDeformedPos) ;
    // fs_Nor = in_weights[0] * ua_bindMats[firstJointID] * ua_bindMats[firstJointID] * vs_Nor;

    // gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    // qt_TexCoord0 = qt_MultiTexCoord0;

    // vec4 blend_pos0 = in_weights[0] * ua_jointMats[in_jointIDs[0]] * ua_bindMats[in_jointIDs[0]]; //more?
    // vec4 blend_pos1 = in_weights[1] * ua_jointMats[in_jointIDs[1]] * ua_bindMats[in_jointIDs[1]];
    // vec4 blend_pos = blend_pos0 + blend_pos1;

    // vec4 modelposition = u_Model * blend_pos;

    // fs_Col = (ua_bindMats[0][3] + vec4(1,1,1,1)) * .5;

    // if(in_jointrIDs[0] == 0) {
    //     //code
    // }

    // gl_Position = u_ViewProj * modelposition;

    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
