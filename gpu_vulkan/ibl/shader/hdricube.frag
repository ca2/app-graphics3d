#version 450

layout(location = 0) in vec3 modelCoordinates;
layout(location = 0) out vec4 FragColor;

// Equirectangular projection HDRI texture
layout(set = 0, binding = 0) uniform sampler2D hdri;

const vec2 inverseAtan = vec2(0.1591, 0.3183);

vec2 sphericalToCartesian(vec3 v)
{
    vec2 xy = vec2(atan(v.z, v.x), asin(v.y));
    xy *= inverseAtan;
    xy += 0.5;
    return xy;
}

void main() {
    vec3 sampleDirection = normalize(modelCoordinates);
    vec2 uv = sphericalToCartesian(sampleDirection);
    vec3 color = texture(hdri, uv).rgb;

    FragColor = vec4(color, 1.0);
}
