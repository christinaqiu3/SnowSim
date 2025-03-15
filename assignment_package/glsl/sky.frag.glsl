#version 150

uniform mat4 u_ViewProj;    // We're actually passing the inverse of the viewproj
                            // from our CPU, but it's named u_ViewProj so we don't
                            // have to bother rewriting our ShaderProgram class

uniform ivec2 u_Dimensions; // Screen dimensions

uniform int u_Time;

uniform vec3 u_PlayerPos;

uniform vec3 u_Light;

out vec4 out_Col;

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


float timeOfDay = mod(float(u_Time)/3.0, 360.0) * 2.f;
vec3 sunColor;

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

//#define RAY_AS_COLOR
//#define SPHERE_UV_AS_COLOR
#define WORLEY_OFFSET

void main()
{
    if (timeOfDay > 190.f) {
        //THIS IS THE MOON COLOR
        sunColor = vec3(210, 200, 255) / 255.0;
    } else {
        sunColor = vec3(255, 255, 190) / 255.0;
    }
    vec2 ndc = (gl_FragCoord.xy / vec2(u_Dimensions)) * 2.0 - 1.0; // -1 to 1 NDC

//    outColor = vec3(ndc * 0.5 + 0.5, 1);

    vec4 p = vec4(ndc.xy, 1, 1); // Pixel at the far clip plane
    p *= 1000.0; // Times far clip plane value
    p = /*Inverse of*/ u_ViewProj * p; // Convert from unhomogenized screen to world

    vec3 rayDir = normalize(p.xyz - u_PlayerPos);

//#ifdef RAY_AS_COLOR
//    out_Col = vec4(0.5 * (rayDir + vec3(1,1,1)), 1.0);
//    return;
//#endif

    vec2 uv = sphereToUV(rayDir);
//#ifdef SPHERE_UV_AS_COLOR
//    out_Col = vec4(uv, 0.0, 1.0);
//    return;
//#endif


    vec2 offset = vec2(0.0);
#ifdef WORLEY_OFFSET
    // Get a noise value in the range [-1, 1]
    // by using Worley noise as the noise basis of FBM
    offset = vec2(worleyFBM(rayDir));
    offset *= 2.0;
    offset -= vec2(1.0);
#endif

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

        //out_Col = vec4(skyColor, 1.0);

        //timeOfDay = mod(float(u_Time/1.5), 360.0);


        // Adjust the position of the sun based on time of day
        float elevation = 0.1 * sin(radians(timeOfDay)); // Adjust the amplitude as needed
        //vec3 sunDir = normalize(vec3(0, elevation, 1.0));
        // Add a glowing sun in the sky
        vec3 sunDir = u_Light;
        float sunSize = 30.0;
        float angle = acos(dot(rayDir, sunDir)) * 360.0 / PI;

        // If the angle between the ray dir and vector to the center of the sun
        // is less than the threshold, then we're looking at the sun
        if (angle < sunSize) {
            // Full center of sun
            if (angle < 7.5) {
                out_Col = vec4(sunColor, 1.0);
            }
            // center of sun, mix with sky color
            else {
                out_Col = vec4(mix(sunColor, skyColor, (angle - 7.5) / 22.5), 1.0);
            }
        } else if (timeOfDay > 150.0 && timeOfDay < duskTime){
            float raySunDot = dot(rayDir, sunDir);
            float SUNSET_THRESHOLD = 0.75;
            float DUSK_THRESHOLD = -0.1;
            if (raySunDot > SUNSET_THRESHOLD) {
                // Do nothing, the sky is already the correct color
            } else if (raySunDot > DUSK_THRESHOLD) {
                // Any dot product between 0.75 and -0.1 is a LERP between sunset and dusk color
                float t = (raySunDot - SUNSET_THRESHOLD) / (DUSK_THRESHOLD - SUNSET_THRESHOLD);
                out_Col = vec4(mix(skyColor, duskColor, t), 1.0);
                //out_Col = vec4(skyColor,1.0);

            } else {
                // Any dot product <= -0.1 is pure dusk color
                out_Col = vec4(duskColor, 1.0);
            }
        } else if (timeOfDay > 330.0 || timeOfDay < sunrise2Time){
            float raySunDot = dot(rayDir, sunDir);
            float SUNSET_THRESHOLD = .75;
            float DUSK_THRESHOLD = -.1;
            if (raySunDot > SUNSET_THRESHOLD) {
                // Do nothing, the sky is already the correct color
            } else if (raySunDot > DUSK_THRESHOLD) {
                // Any dot product between 0.75 and -0.1 is a LERP between sunset and dusk color
                float t = (raySunDot - SUNSET_THRESHOLD) / (DUSK_THRESHOLD - SUNSET_THRESHOLD);
                out_Col = vec4(mix(skyColor, duskColor, t), 1.0);
                //out_Col = vec4(skyColor,1.0);

            } else {
                // Any dot product <= -0.1 is pure dusk color
                out_Col = vec4(duskColor, 1.0);
            }
        } else {
            out_Col = vec4(skyColor, 1.0);
        }
    return;
}
