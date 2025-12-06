#version 330 core

uniform samplerCube skybox;




in vec3 vDirection;
out vec4 outColor;

// Scene UBO
struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec4 viewPos;
    PointLight pointLights[10];
    int numLights;
};


uniform vec3 multiplier;
//uniform float z_multiplier;


void main() {
    vec3 dir = normalize(vDirection);
    //dir.y = y_multiplier * dir.y; // Flip Y if needed should not flip for opengl here given the cubemap generation
    //dir.z = z_multiplier * dir.z;
    dir *= multiplier;
    //outColor = vec4(, 1.0);
        // 1. Fetch HDR color (linear values, can be > 1.0)
    vec3 hdrColor = texture(skybox, dir).rgb;

    // 2. Apply tonemapping
    // Reinhard operator
    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    // Or ACES tonemap (nicer looking, optional):
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    vec3 mapped = clamp((hdrColor*(a*hdrColor+b)) / (hdrColor*(c*hdrColor+d)+e), 0.0, 1.0);

    // 3. Gamma correction (convert from linear → sRGB)
    mapped = pow(mapped, vec3(1.0/2.2));

    outColor = vec4(mapped, 1.0);
}
