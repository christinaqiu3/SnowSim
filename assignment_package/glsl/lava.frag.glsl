#version 330

in vec2 fs_UV;
uniform int u_Time;

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
    vec4 originalColor = texture(u_RenderedTexture, fs_UV);

    vec4 redTint = vec4(1.0, 0.0, 0.0, 0.5); // Adjust the alpha value as needed
    vec2 offset = vec2(WorleyNoise(fs_UV + 1) - WorleyNoise(fs_UV - 1), WorleyNoise(fs_UV + 1) - WorleyNoise(fs_UV - 1));

    color = mix(originalColor, redTint, redTint.a);
    color += texture(u_RenderedTexture, fs_UV + .01 * offset) + (u_Time) / 2;
    color *= vec4(.5, .5, .5, 1.0);

}
