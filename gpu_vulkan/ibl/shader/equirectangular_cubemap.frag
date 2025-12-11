#version 450

layout(location = 0) in vec3 modelCoordinates;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D hdri;

const float PI = 3.14159265359;


void main()
{
//    vec3 dir = normalize(faceRot[pc.faceIndex] * modelCoordinates);

//    float phi   = atan(-dir.z, dir.x);   // longitude: -pi..pi
//    float theta = asin(dir.y);          // latitude: -pi/2..pi/2

//    float u = phi / (2.0 * PI) + 0.5;
//    float v = theta / PI + 0.5;

//    outColor = texture(hdri, vec2(u, v));

    vec3 dir = normalize(modelCoordinates);

    float phi = atan(dir.z, dir.x);
    
    float theta = acos(dir.y);

    float u = mod(0.75 + ((phi + PI) / (2.0 * PI)), 1.0);
    
    float v = theta / PI;
    
    outColor = texture(hdri, vec2(1.0 - u, v));


}
