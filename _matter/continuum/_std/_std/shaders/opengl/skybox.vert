
#version 330 core

layout (location = 0) in vec3 aPos; // Skybox positions
layout (location = 1) in vec3 aNormal; // Skybox positions
layout (location = 2) in vec2 uv; // Skybox positions

out vec3 TexCoords; // Pass to fragment shader

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
 

void main() {
    TexCoords = vec3(aPos.x, aPos.y, -aPos.z); // Flip X
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Keep the z/w ratio 1:1 for a skybox
}
