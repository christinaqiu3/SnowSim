#version 330

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;

out vec4 color;

uniform sampler2D u_RenderedTexture;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)),
                 dot(p, vec2(269.5,183.3))))
                 * 43758.5453);
}

float WorleyNoise(vec2 uv) {
    uv *= 10.0;//change
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            vec2 neighbor = vec2(float(x), float(y)); // Direction in which neighbor cell lies
            vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbors Voronoi point
            float dist = length(diff);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

void main()
{

    //vec3 pixelizedNormal = floor(fs_UV * 10.0) / 10.0;

//    float u = fs_Col[0];
//    float offset = 0;
//    if (fs_Col[2] == 1) {
//        u+= mod(u_Time * .01, 2)*.05;
//        offset = cos(u_Time * .001);
//    }

    vec4 originalColor = texture(u_RenderedTexture, fs_UV);

    vec4 blueTint = vec4(0.0, 0.0, 1.0, 0.5); // Adjust the alpha value as needed

    vec2 offset = vec2(WorleyNoise(fs_UV + 1) - WorleyNoise(fs_UV - 1), WorleyNoise(fs_UV + 1) - WorleyNoise(fs_UV - 1));

    //vec3 newOff = vec3(offset.x, offset.y, 1.f);
    // Mix the original color with the blue tint
    color = mix(originalColor, blueTint, blueTint.a);
    color += texture(u_RenderedTexture, fs_UV + .01 * offset) + (u_Time) / 2;
    color *= vec4(.5, .5, .5, 1.0);
    //color = vec4(pixelizedNormal, texColor.a) * vec4(color, 1.0);



    // Offset the UV coordinates using a whorly noise pattern
        //vec2 offset = vec2(sin(fs_UV.y * 10.0), cos(fs_UV.x * 10.0)) * 0.02;

        // Sample the color from the rendered texture with the offset UV coordinates
        //vec4 texColor = texture(u_RenderedTexture, fs_UV + offset);

        // Apply additional modulation to the RGB values based on time
        //vec3 newColor = texColor.rgb + offset * cos(u_Time * 0.01) * 0.5;

        // Combine the original color with the modified color
        //vec4 Ncolor = vec4(newColor, texColor.a);
        //color = mix(Ncolor, blueTint, blueTint.a);


}
