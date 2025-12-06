#version 450

layout(location = 0) in vec3 modelCoordinates;
layout(location = 0) out vec4 FragColor;

// Equirectangular projection HDRI texture
layout(set = 0, binding = 0) uniform sampler2D hdri;

vec2 sphericalToCartesian(vec3 v)
{
    float phi = atan(v.z, -v.x);       // horizontal angle
    float theta = asin(v.y);          // vertical angle

    // normalize to 0..1
    vec2 uv;
    uv.x = 0.5 + phi * (0.15915494309189535);   // 1 / (2π)
    uv.y = 0.5 + theta * (0.3183098861837907);  // 1 / π (note the minus!)

    return uv;
}

void main() {
	vec3 sampleDirection = normalize(modelCoordinates);
	vec2 uv = sphericalToCartesian(sampleDirection);
	vec3 color = texture(hdri, uv).rgb;

	FragColor = vec4(color, 1.0);
}