#version 420 core
 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
 
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;
 
struct PointLight {
    vec4 position;
    vec4 color;
};
 
// Must match the same binding point as in your C++ side using glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboBuffer)
layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
    int padding1;
    int padding2;
    int padding3;
};
 
// Instead of push constants, use a second uniform block or separate uniforms
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
 
void main() {
    vec4 positionWorld = modelMatrix * vec4(position, 1.0);
    gl_Position = projection * view * positionWorld;
 
    fragNormalWorld = normalize(mat3(normalMatrix) * normal);
    fragPosWorld = positionWorld.xyz;
    fragColor = color;
}
 