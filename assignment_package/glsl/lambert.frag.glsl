#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.

uniform vec4 u_Color; // The color with which to render this instance of geometry.

uniform sampler2D u_Texture; // The texture to be read from by this shader

uniform sampler2D u_ShadowTexture;

uniform int u_Time; //Who's time is it? it's u_Time

uniform sampler2D u_norMapTexture;

uniform vec3 u_PlayerPos;

uniform vec3 u_Light;

uniform mat4 u_ViewProj;    // We're actually passing the inverse of the viewproj
                            // from our CPU, but it's named u_ViewProj so we don't
                            // have to bother rewriting our ShaderProgram class

uniform ivec2 u_Dimensions; // Screen dimensions


// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Pos;
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;

in vec2 fs_uv;
in vec4 fs_ll;
in float anim;
in vec4 fs_shadowPos;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

const float PI = 3.14159265359;
const float TWO_PI = 6.28318530718;

// Nighttime palette
    const vec3 dark[5] = vec3[](vec3(0, 0, 0),
                                      vec3(0, 0, 0),
                                      vec3(0, 0, 0),
                                      vec3(0, 0, 0),
                                      vec3(0, 0, 0));

    // Daytime palette
    const vec3 day[5] = vec3[](vec3(135, 206, 250) / 255.0,
                                        vec3(135, 206, 250) / 255.0,
                                        vec3(173, 216, 230) / 255.0,
                                        vec3(240, 248, 255) / 255.0,
                                        vec3(240, 248, 255) / 255.0);

    // Sunset palette
    const vec3 sunset[5] = vec3[](vec3(255, 229, 119) / 255.0,
                                   vec3(254, 192, 81) / 255.0,
                                   vec3(255, 137, 103) / 255.0,
                                   vec3(253, 96, 81) / 255.0,
                                   vec3(57, 32, 51) / 255.0);
    // Dusk palette
    const vec3 dusk[5] = vec3[](vec3(144, 96, 144) / 255.0,
                                vec3(96, 72, 120) / 255.0,
                                vec3(72, 48, 120) / 255.0,
                                vec3(48, 24, 96) / 255.0,
                                vec3(0, 24, 72) / 255.0);



const vec3 sunColor = vec3(255, 255, 190) / 255.0;
const vec3 cloudColor = sunset[3];

vec2 sphereToUV(vec3 p) {
    float phi = atan(p.z, p.x);
    if(phi < 0) {
        phi += TWO_PI;
    }
    float theta = acos(p.y);
    return vec2(1 - phi / TWO_PI, 1 - theta / PI);
}

vec3 uvToSunset(vec2 uv) {
    if(uv.y < 0.5) {
        return sunset[0];
    }
    else if(uv.y < 0.55) {
        return mix(sunset[0], sunset[1], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(sunset[1], sunset[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(sunset[2], sunset[3], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(sunset[3], sunset[4], (uv.y - 0.65) / 0.1);
    }
    return sunset[4];
}

vec3 uvFromSunset(vec2 uv) {
    if(uv.y < 0.5) {
        return sunset[4];
    }
    else if(uv.y < 0.55) {
        return mix(sunset[4], sunset[3], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(sunset[3], sunset[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(sunset[2], sunset[1], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(sunset[1], sunset[0], (uv.y - 0.65) / 0.1);
    }
    return sunset[0];
}

vec3 uvToDusk(vec2 uv) {
    if(uv.y < 0.5) {
        return dusk[0];
    }
    else if(uv.y < 0.55) {
        return mix(dusk[0], dusk[1], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(dusk[1], dusk[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(dusk[2], dusk[3], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(dusk[3], dusk[4], (uv.y - 0.65) / 0.1);
    }
    return dusk[4];
}

vec3 uvFromDusk(vec2 uv) {
    if(uv.y < 0.5) {
        return dusk[4];
    }
    else if(uv.y < 0.55) {
        return mix(dusk[4], dusk[3], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(dusk[3], dusk[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(dusk[2], dusk[1], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(dusk[1], dusk[0], (uv.y - 0.65) / 0.1);
    }
    return dusk[0];
}

vec3 uvToFromDark(vec2 uv) {
    if(uv.y < 0.5) {
        return dark[4];
    }
    else if(uv.y < 0.55) {
        return mix(dark[4], dark[3], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(dark[3], dark[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(dark[2], dark[1], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(dark[1], dark[0], (uv.y - 0.65) / 0.1);
    }
    return dark[0];
}

vec3 uvToFromDay(vec2 uv) {
    if(uv.y < 0.5) {
        return day[4];
    }
    else if(uv.y < 0.55) {
        return mix(day[4], day[3], (uv.y - 0.5) / 0.05);
    }
    else if(uv.y < 0.6) {
        return mix(day[3], day[2], (uv.y - 0.55) / 0.05);
    }
    else if(uv.y < 0.65) {
        return mix(day[2], day[1], (uv.y - 0.6) / 0.05);
    }
    else if(uv.y < 0.75) {
        return mix(day[1], day[0], (uv.y - 0.65) / 0.1);
    }
    return day[0];
}

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec3 random3( vec3 p ) {
    return fract(sin(vec3(dot(p,vec3(127.1, 311.7, 191.999)),
                          dot(p,vec3(269.5, 183.3, 765.54)),
                          dot(p, vec3(420.69, 631.2,109.21))))
                 *43758.5453);
}

float WorleyNoise3D(vec3 p)
{
    // Tile the space
    vec3 pointInt = floor(p);
    vec3 pointFract = fract(p);

    float minDist = 1.0; // Minimum distance initialized to max.

    // Search all neighboring cells and this cell for their point
    for(int z = -1; z <= 1; z++)
    {
        for(int y = -1; y <= 1; y++)
        {
            for(int x = -1; x <= 1; x++)
            {
                vec3 neighbor = vec3(float(x), float(y), float(z));

                // Random point inside current neighboring cell
                vec3 point = random3(pointInt + neighbor);

                // Animate the point
                point = 0.5 + 0.5 * sin(u_Time * 0.01 + 6.2831 * point); // 0 to 1 range

                // Compute the distance b/t the point and the fragment
                // Store the min dist thus far
                vec3 diff = neighbor + point - pointFract;
                float dist = length(diff);
                minDist = min(minDist, dist);
            }
        }
    }
    return minDist;
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

float worleyFBM(vec3 uv) {
    float sum = 0;
    float freq = 4;
    float amp = 0.5;
    for(int i = 0; i < 8; i++) {
        sum += WorleyNoise3D(uv * freq) * amp;
        freq *= 2;
        amp *= 0.5;
    }
    return sum;
}


vec3 getSkyColor(){
    vec2 ndc = (gl_FragCoord.xy / vec2(u_Dimensions)) * 2.0 - 1.0; // -1 to 1 NDC

    vec4 p = vec4(ndc.xy, 1, 1); // Pixel at the far clip plane
    p *= 1000.0; // Times far clip plane value
    p = /*Inverse of*/ u_ViewProj * p; // Convert from unhomogenized screen to world

    vec3 rayDir = normalize(p.xyz - u_PlayerPos);
    vec2 uv = sphereToUV(rayDir);

    vec2 offset = vec2(0.0);

    // Compute a gradient from the bottom of the sky-sphere to the top
    vec3 sunsetColor = uvToSunset(uv + offset * 0.1);
    vec3 duskColor = uvToDusk(uv + offset * 0.1);
    vec3 sunrise1Color = uvFromDusk(uv + offset * 0.1);
    vec3 sunrise2Color = uvFromSunset(uv + offset * 0.1);
    vec3 nightColor = uvToFromDark(uv + offset * 0.1);
    vec3 dayColor = uvToFromDay(uv + offset * 0.1);

    //out_Col = vec4(sunsetColor, 1.0);

    // Time-dependent sun position
    float timeOfDay = mod(float(u_Time/3.0), 360.0) * 2;

    // Define time intervals for transitions
    float sunrise1Time = 0.0;
    float sunrise2Time = 30.0;
    float dayTime = 60.0;
    float sunsetTime = 180.0;
    float duskTime = 210.0;
    float nightTime = 240.0;

    vec3 skyColor;


    // Interpolate between color palettes based on time of day
    if (timeOfDay < sunrise2Time) {
        // Sunrise1 to sunrise2
        float t = smoothstep(sunrise1Time, sunrise2Time, timeOfDay);
        skyColor = mix(duskColor, sunsetColor, t);
    } else if (timeOfDay < dayTime) {
        // sunrise2 to daytime
        float t = smoothstep(sunrise2Time, dayTime, timeOfDay);
        skyColor = mix(sunsetColor, dayColor, t);
    } else if (timeOfDay >= dayTime && timeOfDay <= 150.0) {
        skyColor = dayColor;
    } else if (timeOfDay < sunsetTime) {
        // Daytime to sunset
        float t = smoothstep(150.0, sunsetTime, timeOfDay);
        skyColor = mix(dayColor, sunsetColor, t);
    } else if (timeOfDay < duskTime) {
        // Sunset to dusk
        float t = smoothstep(sunsetTime, duskTime, timeOfDay);
        skyColor = mix(sunsetColor, duskColor, t);
    } else if (timeOfDay < nightTime) {
        // Dusk to night
        float t = smoothstep(duskTime, nightTime, timeOfDay); // transition from dusk to night
        skyColor = mix(duskColor, nightColor, t);
    } else if (timeOfDay >= nightTime && timeOfDay <= 330.0) {
        skyColor = nightColor;
    } else if (timeOfDay > 330.0){
        float t = smoothstep(330.0, 360.0, timeOfDay);
        skyColor = mix(nightColor, duskColor, t);
    }

    return skyColor;
}

float random1(vec3 p) {
    return fract(sin(dot(p,vec3(127.1, 311.7, 191.999)))
                 *43758.5453);
}

float mySmoothStep(float a, float b, float t) {
    t = smoothstep(0, 1, t);
    return mix(a, b, t);
}

float cubicTriMix(vec3 p) {
    vec3 pFract = fract(p);
    float llb = random1(floor(p) + vec3(0,0,0));
    float lrb = random1(floor(p) + vec3(1,0,0));
    float ulb = random1(floor(p) + vec3(0,1,0));
    float urb = random1(floor(p) + vec3(1,1,0));

    float llf = random1(floor(p) + vec3(0,0,1));
    float lrf = random1(floor(p) + vec3(1,0,1));
    float ulf = random1(floor(p) + vec3(0,1,1));
    float urf = random1(floor(p) + vec3(1,1,1));

    float mixLoBack = mySmoothStep(llb, lrb, pFract.x);
    float mixHiBack = mySmoothStep(ulb, urb, pFract.x);
    float mixLoFront = mySmoothStep(llf, lrf, pFract.x);
    float mixHiFront = mySmoothStep(ulf, urf, pFract.x);

    float mixLo = mySmoothStep(mixLoBack, mixLoFront, pFract.z);
    float mixHi = mySmoothStep(mixHiBack, mixHiFront, pFract.z);

    return mySmoothStep(mixLo, mixHi, pFract.y);
}

float fbm(vec3 p) {
    float amp = 0.5;
    float freq = 4.0;
    float sum = 0.0;
    for(int i = 0; i < 8; i++) {
        sum += cubicTriMix(p * freq) * amp;
        amp *= 0.5;
        freq *= 2.0;
    }
    return sum;
}

bool isWaterUV(vec2 uv) {
    vec2 d1 = abs(uv - vec2(0.8125f, 0.1875f));
    vec2 d2 = abs(uv - vec2(0.875, 0.1875f));
    vec2 d3 = abs(uv - vec2(0.875, 0.25));
    vec2 d4 = abs(uv - vec2(0.8125f, 0.25));
    float dP = 0.00390625 * 16.0;
    return (abs(uv.x - 0.8125) < dP) && (abs(uv.y - 0.1875) < dP);
    //return length(d1) < dP || length(d2) < dP || length(d3) < dP || length(d4) < dP;

}


void main()
{
    // Material base color (before shading)
    //vec4 diffuseColor = fs_Col;
    //TAKES THE COLOR AND ADDS SOME BASIC NOISE
    //WE WANT THIS TO USE THE TEXTURE COLOR NOT THE BASE COLOR
    //diffuseColor = diffuseColor * (0.5 * fbm(fs_Pos.xyz) + 0.5);
//    float u = fs_Col[0];
//    if  (fs_Col[2] == 1) {
//        u += mod(u_Time*0.01, 2)*0.0625;
//    }

    vec3 currSkyCol = getSkyColor();

    //NORMAL MAP (DON"T USE IT)
    vec4 norm = vec4(texture(u_norMapTexture, fs_uv).rbg, 1);
    norm = normalize(norm * 2.0 - 1.0);

    // Calculate the diffuse term for Lambert shading
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
    // Avoid negative lighting values
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    //THIS APPLIES THE UV OFFSED FOR WATER AND LAVA
    float u = fs_uv[0];
    if (anim > 0) {
        u += mod(u_Time*0.01, 2)*0.0625;
    }

    //BASE COLORING FROM TEXTURE
    vec4 diffuseColor = texture(u_Texture, vec2(u, fs_uv[1]));
    if (anim < 0 && diffuseColor.g > diffuseColor.r) {
        diffuseColor = vec4(vec3(clamp((diffuseColor.r + diffuseColor.g + diffuseColor.b)/2.0, 0.0, 1.0)), 1.0);
        float temperature = (floor(anim)) / 256.0; //((floor(fs_Col[2]) >= -1000 ? floor(fs_Col[2] / 1000.f) : floor(fs_Col[2])) / 256.0);
        float humidity = fract(anim);
        diffuseColor = vec4((humidity * vec3(0, 0.8, 0.0) + (1.0-humidity) * (temperature * vec3(0.47, 0.9, 0.6) + (1.0-temperature) * vec3(0.79, 0.97, 0.25))) * diffuseColor.x, 1.0);

    }

    if (isWaterUV(fs_uv.xy)) {
        // Lambertian shading
        float diffuseTerm = max(dot(normalize(fs_Nor.xyz), normalize(fs_LightVec.xyz)), 0.0);

        // Specular reflection
        vec3 viewDir = normalize(u_PlayerPos - fs_Pos.xyz);
        vec3 reflectDir = reflect(-fs_LightVec.xyz, normalize(fs_Nor.xyz));
        float specTerm = pow(max(dot(viewDir, reflectDir), 0.0), 10.0);

        // Combine Lambertian and specular terms
        vec4 specularColor = diffuseColor; // Replace with your actual specular color
        float shininess = 30.0; // Adjust the shininess factor
        out_Col = diffuseColor * diffuseTerm + specularColor * specTerm;

        float distance =  length(fs_Pos.xz-u_PlayerPos.xz);
        float fogFactor = smoothstep(0.0, 200.0, distance);
        vec3 fogColor = mix(out_Col.rgb, currSkyCol, fogFactor);

        out_Col = vec4(fogColor, .66);
        return;
    }


    float bias = max(0.014 * (1 - dot(fs_Nor, fs_LightVec)), 0.0028);

    float visibility = 1.0;
    vec2 poissonDisk[4] = vec2[](
      vec2( -0.64201624, -0.39906216 ),
      vec2( 0.54558609, -0.46890725 ),
      vec2( -0.094184101, -0.92938870 ),
      vec2( 0.34495938, 0.29387760 )
    );

    for (int i=0;i<4;i++){
        //int index = int(16.0*random1(gl_FragCoord.xyy + fbm(fs_Pos.xyz)))%16;
        if ( texture( u_ShadowTexture, fs_shadowPos.xy + poissonDisk[i]*0.0002).z  <  fs_shadowPos.z-bias ){
          visibility-=0.15;
        }
    }
    if (mod(float(u_Time/3.0), 360.0) * 2  > 190.f) {
        visibility = 0.2;
    }


    diffuseColor *= fs_Col;

    float ambientTerm = 0.2;

    //CALCULATES LAMBERT SHADOW AND SHADOW MAP SHADOW
    float lightIntensity = (diffuseTerm + ambientTerm) * visibility;

    if (lightIntensity < 0.2) {
        lightIntensity = 0.2;
    }
    diffuseColor *= max(lightIntensity, fs_ll.x / 15.f);
    //vec3 test = u_PlayerPos;
    //float startDistance = 3.0;
    float distance =  length(fs_Pos.xz-u_PlayerPos.xz);
    //float fogFactor = clamp((distance) / 10.0, 0.0, 1.0);
    float fogFactor = smoothstep(0.0, 200.0, distance);
    vec3 fogColor = mix(diffuseColor.rgb, currSkyCol, fogFactor);
    // Compute final shaded color
    // NO DISTANCE FOG SHADER
    //out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a);
    
    if (diffuseColor.a == 0.f) {
        discard;
    }
    //out_Col = vec4(fogColor * lightIntensity, diffuseColor.a);


    out_Col = vec4(fogColor, 1.0);
}
